import os
import re

def find_libraries(path):
    include_pattern = re.compile(r'#\s*include\s*<([^>]+)>')
    libraries = set()

    print(f"Starting scan in directory: {path}")
    print(f"Current working directory: {os.getcwd()}")  # Affiche le répertoire de travail courant

    # Afficher l'arborescence des dossiers
    for root, dirs, files in os.walk(path):
        print(f"Scanning directory: {root}")
        print("Directories:", dirs)
        print("Files:", files)

        for file in files:
            if file.endswith(('.ino', '.h', '.hpp')):
                filepath = os.path.join(root, file)
                print(f"Reading file: {filepath}")  # Affiche chaque fichier lu

                try:
                    with open(filepath, 'r', encoding='utf-8') as file:
                        for line in file:
                            match = include_pattern.search(line)
                            if match:
                                lib_name = match.group(1)
                                lib_name = os.path.splitext(os.path.basename(lib_name))[0]
                                libraries.add(lib_name)
                                print(f"Found library: {lib_name}")  # Affiche chaque bibliothèque trouvée
                except Exception as e:
                    print(f"Error reading file {filepath}: {e}")

    return libraries

def write_libraries_to_file(libraries, filename):
    print(f"Writing to file: {filename}")
    os.makedirs(os.path.dirname(filename), exist_ok=True)

    with open(filename, 'w') as file:
        for lib in sorted(libraries):
            file.write(f"{lib}\n")
            print(f"Writing library: {lib}")  # Affiche les bibliothèques écrites

# Déterminer le chemin absolu du dossier source
script_directory = os.path.dirname(os.path.abspath(__file__))
project_path = os.path.join(script_directory, '../esp-wake-computer')
libraries_file = os.path.join(script_directory, '../bin/required-libraries.txt')

print(f"Script directory: {script_directory}")
print(f"Project path: {project_path}")
print(f"Libraries file path: {libraries_file}")

libraries = find_libraries(project_path)
write_libraries_to_file(libraries, libraries_file)

print(f"Libraries extracted and written to {libraries_file}")
