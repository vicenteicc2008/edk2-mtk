#!/usr/bin/env python3
import os
import subprocess
import argparse
from pathlib import Path

MAX_PACKED_SIZE = 16 * 1024 * 1024  # 16MB
MAGIC_HEADER = 0x8A7A6A5A

def compress_file(input_path: Path) -> tuple[Path, int, int]:
    """Compress file using LZMA and return compressed path, original and compressed sizes."""
    compressed_path = input_path.with_suffix(input_path.suffix + '.lzma')
    
    try:
        subprocess.run(['lzma', '-kf', str(input_path)], check=True)
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"Failed to compress file: {e}") from e
    
    uncompressed_size = input_path.stat().st_size
    compressed_size = compressed_path.stat().st_size
    
    return compressed_path, uncompressed_size, compressed_size

def create_packed_file(output_path: Path, compressed_path: Path, uncompressed_size: int, compressed_size: int):
    """Create packed file with magic header and compressed data."""
    if (compressed_size + 0xc) > MAX_PACKED_SIZE:
        raise ValueError(f"Compressed size {compressed_size} exceeds maximum allowed size {MAX_PACKED_SIZE}")
    
    with output_path.open('wb') as packed_file, compressed_path.open('rb') as compressed_file:
        # Write header (12 bytes)
        packed_file.write(MAGIC_HEADER.to_bytes(4, byteorder='little'))
        packed_file.write(uncompressed_size.to_bytes(4, byteorder='little'))
        packed_file.write(compressed_size.to_bytes(4, byteorder='little'))
        
        # Write compressed data
        packed_file.write(compressed_file.read())

def main():
    parser = argparse.ArgumentParser(description='Pack a file with LZMA compression and custom header')
    parser.add_argument('input', help='Input file to process')
    parser.add_argument('-o', '--output', default='ramdisk.cpio', help='Output packed file path')
    
    args = parser.parse_args()
    
    input_path = Path(args.input)
    output_path = Path(args.output)
    
    if not input_path.exists():
        raise FileNotFoundError(f"Input file not found: {input_path}")
    
    try:
        compressed_path, uncompressed_size, compressed_size = compress_file(
            input_path
        )
        create_packed_file(
            output_path, compressed_path, uncompressed_size, compressed_size
        )
        print(f"Successfully created packed file: {output_path}")
    except Exception as e:
        print(f"Error: {e}")
        raise SystemExit(1)

if __name__ == '__main__':
    main()