import os
import subprocess

def format_cpp_files(root_dir):
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.endswith(('.cpp', '.h')):
                file_path = os.path.join(dirpath, filename)
                print(f'Formatting {file_path}')
                subprocess.run(['clang-format', '-i', file_path])

if __name__ == '__main__':
    format_cpp_files('./source')
    format_cpp_files('./test')