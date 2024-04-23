#!/usr/bin/env python3
# -*- coding: utf-8 -*-
##
# @file set_catalog_config.py
# @brief source [board] and [src] Kconfig file
# @author Tuya
# @version 1.0.0
# @date 2023-04-10


import os
import argparse


KCONFIG = "Kconfig"


def set_config(board, src_dir, output_config):
    context = "# CatalogKconfig\n"
    config_path = os.path.join(src_dir, KCONFIG)
    if os.path.exists(config_path):
        context += f'source \"{config_path}\"\n'
    if board:
        config_path = os.path.join(board, KCONFIG)
        if os.path.exists(config_path):
            context += f'source \"{config_path}\"\n'

    with open(output_config, 'w', encoding='utf-8') as f:
        f.write(context)
    return True


def main():
    parse = argparse.ArgumentParser(
        usage="-b <board> -s src -o cache/Kconfig",
        description="Generate the Kconfig catalog.")
    parse.add_argument('-b', '--board', type=str, default=None,
                       help="Board name. [None]", metavar="")
    parse.add_argument('-s', '--src', type=str, default="src",
                       help="Src directory. [src]", metavar="")
    parse.add_argument('-o', '--output', type=str, default="cache/Kconfig",
                       help="Output file of Kconfig. [cache/Kconfig]",
                       metavar="")
    args = parse.parse_args()

    board = args.board
    src_dir = args.src
    output_config = args.output
    print(f'board: {board}')
    print(f'src_dir: {src_dir}')
    print(f'output_config: {output_config}')
    output_dir = os.path.dirname(output_config)
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    set_config(board, src_dir, output_config)
    pass


if __name__ == '__main__':
    main()
    pass
