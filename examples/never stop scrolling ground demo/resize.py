import os
from PIL import Image
import glob

def resize_pngs(input_dir=None, output_dir=None, width=16, height=16, keep_aspect_ratio=True):
    """
    Resize all PNG files in a directory to specified dimensions while preserving colors and transparency
    """
    
    # Set input directory
    if input_dir is None:
        input_dir = os.getcwd()
    
    # Set output directory
    if output_dir is None:
        output_dir = input_dir
    else:
        os.makedirs(output_dir, exist_ok=True)
    
    # Find all PNG files
    png_pattern = os.path.join(input_dir, "*.png")
    png_files = glob.glob(png_pattern)
    
    if not png_files:
        print(f"No PNG files found in {input_dir}")
        return
    
    print(f"Found {len(png_files)} PNG files to process...")
    
    # Process each PNG file
    for png_file in png_files:
        # Skip icon.png
        if os.path.basename(png_file).lower() == 'icon.png':
            print(f"Skipping: {os.path.basename(png_file)}")
            continue
            
        try:
            # Open image and preserve original mode (RGBA, RGB, etc.)
            with Image.open(png_file) as img:
                original_mode = img.mode
                
                # Get original dimensions
                orig_width, orig_height = img.size
                
                if keep_aspect_ratio:
                    # Resize while maintaining aspect ratio
                    img.thumbnail((width, height), Image.Resampling.LANCZOS)
                    resized_img = img
                else:
                    # Resize to exact dimensions (may distort aspect ratio)
                    resized_img = img.resize((width, height), Image.Resampling.LANCZOS)
                
                # Determine output filename
                if output_dir == input_dir:
                    output_path = png_file
                else:
                    filename = os.path.basename(png_file)
                    output_path = os.path.join(output_dir, filename)
                
                # Save with original mode to preserve colors and transparency
                resized_img.save(output_path, "PNG")
                print(f"Processed: {os.path.basename(png_file)} ({orig_width}x{orig_height} -> {resized_img.size[0]}x{resized_img.size[1]}, mode: {original_mode})")
                
        except Exception as e:
            print(f"Error processing {png_file}: {str(e)}")
    
    print("Processing completed!")

def main():
    """Main function with example usage"""
    
    # Process PNGs in current directory (overwrite originals)
    resize_pngs()
    
    # Alternative: Process from one directory to another with custom size
    # resize_pngs(
    #     input_dir="./source_images",
    #     output_dir="./processed_images",
    #     width=38,
    #     height=58,
    #     keep_aspect_ratio=True  # Set to False for exact dimensions
    # )

if __name__ == "__main__":
    main()