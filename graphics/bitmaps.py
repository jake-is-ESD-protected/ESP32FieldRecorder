from PIL import Image
import numpy as np
import os

def load_image(image_path):
    # Load the image
    image = Image.open(image_path)

    # Convert the image to grayscale
    image = image.convert('1')

    return np.array(image)

def convert_to_c_bitmap(image_array):
    # Convert the boolean values to integers and then to a string of 0s and 1s
    binary_string = ''.join(map(str, image_array.flatten().astype(int)))

    # Split the binary string into chunks of 8 bits
    bytes = [binary_string[i:i+8] for i in range(0, len(binary_string), 8)]

    # Convert each byte to its hexadecimal representation
    hex_bytes = [f'0x{int(b, 2):02x}' for b in bytes]

    return ', '.join(hex_bytes)

def save_to_cpp_file(cpp_file_path, h_file_name, bitmap, array_name):
    # Create the CPP file
    with open(cpp_file_path, 'w') as f:
        f.write(f'#include "{h_file_name}"\n\n')
        f.write(f'const unsigned char {array_name}[] = {{\n{bitmap}\n}};')

def save_to_h_file(h_file_path, array_name):
    # Create the H file
    basename = os.path.basename(h_file_path)
    basename = os.path.splitext(basename)[0]
    with open(h_file_path, 'w') as f:
        f.write(f'#ifndef {basename.upper()}_H\n')
        f.write(f'#define {basename.upper()}_H\n\n')
        f.write(f'extern const unsigned char {array_name}[];\n\n')
        f.write(f'#endif // {basename.upper()}_H')

def convert_png_to_c_bitmap(image_path, cpp_file_path, h_file_path):
    # Load the image and convert it to a C bitmap
    image_array = load_image(image_path)
    array_name = os.path.splitext(os.path.basename(image_path))[0]
    c_bitmap = convert_to_c_bitmap(image_array)

    # Save the C bitmap to a CPP file and a H file
    save_to_cpp_file(cpp_file_path, os.path.basename(h_file_path), c_bitmap, array_name)
    save_to_h_file(h_file_path, array_name)

# Test the function
convert_png_to_c_bitmap('graphics/title_screen.png', 'lib/oled/bmp_title_screen.cpp', 'lib/oled/bmp_title_screen.h')
