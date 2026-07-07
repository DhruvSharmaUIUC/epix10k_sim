import os

# Target directories to scrape
TARGET_DIRS = ['/sdf/home/d/dhruvshr/ePix_Simulation/Examples/PixelSim_ePix/src', '/sdf/home/d/dhruvshr/ePix_Simulation/Examples/PixelSim_ePix/include']
OUTPUT_FILE = 'llm_context.txt'

# Extensions to skip (add or remove as needed)
IGNORE_EXTENSIONS = {'.png', '.jpg', '.jpeg', '.gif', '.ico', '.pdf', '.o', '.a', '.so', '.dll', '.exe'}
# Specific files or hidden names to skip
IGNORE_FILES = {'.DS_Store', 'Thumbs.db'}

def build_tree(dirs):
    """Generates a text-based tree representation of the folders."""
    tree = ["# Codebase Structure\n```"]
    for d in dirs:
        if not os.path.exists(d):
            continue
        tree.append(f"{d}/")
        for root, _, files in os.walk(d):
            level = root.replace(d, '').count(os.sep)
            indent = ' ' * 4 * (level + 1)
            sub_folder = os.path.basename(root)
            if sub_folder and sub_folder != d:
                tree.append(f"{' ' * 4 * level}{sub_folder}/")
            for f in files:
                if f in IGNORE_FILES or os.path.splitext(f)[1] in IGNORE_EXTENSIONS:
                    continue
                tree.append(f"{indent}{f}")
    tree.append("```\n" + "="*50 + "\n")
    return "\n".join(tree)

def main():
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as outfile:
        # 1. Inject tree structure so the LLM understands folder hierarchy
        outfile.write(build_tree(TARGET_DIRS))
        
        # 2. Append contents with clean XML/Markdown tags
        for target in TARGET_DIRS:
            if not os.path.exists(target):
                continue
            for root, _, files in os.walk(target):
                for file in files:
                    ext = os.path.splitext(file)[1].lower()
                    if file in IGNORE_FILES or ext in IGNORE_EXTENSIONS:
                        continue
                        
                    file_path = os.path.join(root, file)
                    # Normalize backslashes for Windows users
                    normalized_path = file_path.replace("\\", "/") 
                    
                    try:
                        with open(file_path, 'r', encoding='utf-8', errors='ignore') as infile:
                            # Use explicit XML-style tags for absolute clarity
                            outfile.write(f'<file path="{normalized_path}">\n')
                            outfile.write(infile.read())
                            outfile.write(f'\n</file>\n\n')
                    except Exception as e:
                        print(f"Skipping {normalized_path}: {e}")

    print(f"LLM Context file successfully generated: {OUTPUT_FILE}")

if __name__ == '__main__':
    main()
