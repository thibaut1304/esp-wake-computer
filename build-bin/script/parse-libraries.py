import os
import re
import subprocess
import json

def find_libraries(path):
    include_pattern = re.compile(r'#\s*include\s*<([^>]+)>')
    libraries = set()

    print(f"Starting scan in directory: {path}")
    print(f"Current working directory: {os.getcwd()}")

    for root, dirs, files in os.walk(path):
        print(f"Scanning directory: {root}")
        print("Directories:", dirs)
        print("Files:", files)

        for file in files:
            if file.endswith(('.ino', '.h', '.hpp')):
                filepath = os.path.join(root, file)
                print(f"Reading file: {filepath}")

                try:
                    with open(filepath, 'r', encoding='utf-8') as file:
                        for line in file:
                            match = include_pattern.search(line)
                            if match:
                                lib_name = match.group(1)
                                lib_name = os.path.splitext(os.path.basename(lib_name))[0]
                                libraries.add(lib_name)
                                # print(f"Found library: {lib_name}")
                except Exception as e:
                    print(f"Error reading file {filepath}: {e}")

    return libraries

def get_installed_libraries():
    """Récupère les bibliothèques déjà installées et les renvoie sous forme de set."""
    result = subprocess.run(['/workspace/esp-wake-computer/arduino-cli', 'lib', 'list', '--format', 'json'], capture_output=True, text=True)
    installed_libraries = set()
    if result.stdout:
        data = json.loads(result.stdout)
        for lib in data:
            installed_libraries.add(lib['library']['name'])
    return installed_libraries

def find_library_names(headers):
	library_names = set()
	for header in headers:
		result = subprocess.run(['/workspace/esp-wake-computer/arduino-cli', 'lib', 'search', header, '--format', 'json'], capture_output=True, text=True)
		if result.stdout:
			try:
				lib_info = json.loads(result.stdout)
				if 'libraries' in lib_info and lib_info['libraries']:
					for lib in lib_info['libraries']:
						# Check if architecture esp32 is present
						# if any('esp32' in arch for arch in lib.get('architectures', [])):
						if 'name' in lib:
							library_names.add(lib['name'])
							# print(f"Matched library: {lib['name']} for header {header}")
				else:
					print(f"No libraries found for header: {header}")
			except json.JSONDecodeError:
				print(f"Failed to parse JSON from arduino-cli output for header: {header}")
		else:
			print(f"No output from arduino-cli for header: {header}")
	return library_names



def write_libraries_to_file(libraries, filename, installed_libraries):
	print(f"Writing to file: {filename}")
	os.makedirs(os.path.dirname(filename), exist_ok=True)

	with open(filename, 'w') as file:
		for lib in sorted(libraries):
			if lib not in installed_libraries:
				file.write(f"{lib}\n")
				# print(f"Writing library: {lib}")

script_directory = os.path.dirname(os.path.abspath(__file__))
project_path = os.path.join(script_directory, '/workspace/esp-wake-computer')
libraries_file = os.path.join(script_directory, '/workspace/bin/required-libraries.txt')

print(f"Script directory: {script_directory}")
print(f"Project path: {project_path}")
print(f"Libraries file path: {libraries_file}")

headers = find_libraries(project_path)
print(f"Headers found : {headers}")
installed_libraries = get_installed_libraries()
print(f"Libraries installed found : {installed_libraries}")
libraries = find_library_names(headers)
print(f"Libraries found : {libraries}")
write_libraries_to_file(libraries, libraries_file, installed_libraries)

print(f"Libraries extracted and written to {libraries_file}")
