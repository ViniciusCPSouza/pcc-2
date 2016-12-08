#!/usr/bin/env python

"""
Script to generate the data for testing strnig searching algorithms.

<line_size> The size of each line'.
<data_set_name> The name of the generated dataset. If it is set to 'auto', the parameters will decide the name.

Usage:
    data_gen.py <line_size> <dataset_name>
"""

import math
import os
import random
import shutil
import string

from docopt import docopt


SIZE_SM = "sm"
SIZE_LG = "lg"

ALPHABETS = {"sm": ["A", "C", "G", "T"],
             "lg": string.ascii_letters}

TEXT_FILES_COUNT = 1
TEXT_LINES_COUNT = 100

OUTPUT_FOLDER = "datasets"


def main(args):
    a_size = "lg"
    line_size = int(args['<line_size>'])
    dataset_name = args['<dataset_name>']

    get_chars = lambda alphabet, n: (random.choice(alphabet) for _ in range(n))

    if dataset_name == "auto":
        dataset_name = "%d_ls" % line_size

    dataset_folder = os.path.join(OUTPUT_FOLDER, dataset_name)
    text_folder = os.path.join(dataset_folder, "txt")
    patterns_filepath = os.path.join(dataset_folder, "patterns.txt")

    if os.path.exists(dataset_folder):
        shutil.rmtree(dataset_folder)

    os.makedirs(dataset_folder)
    os.makedirs(text_folder)

    patterns = ["".join(get_chars(ALPHABETS[a_size], 100)) for _ in range(10)]

    # generate pattern
    with open(patterns_filepath, "w") as f:
        for p in patterns:
            f.write(p)
            f.write("\n")

    # generate texts
    for i in range(TEXT_FILES_COUNT):
        with open(os.path.join(text_folder, "text%d.txt" % i), "w") as f:
            for _ in range(TEXT_LINES_COUNT):
                num_matches = int(line_size / 100.0)
                remaining_chars = line_size - (num_matches * 100)
                line = []

                for _ in range(num_matches):
                    prefix = random.randint(0, remaining_chars)
                    if prefix:
                        remaining_chars -= prefix
                        line.extend(get_chars(ALPHABETS[a_size], prefix))
                    match = random.choice(patterns)
                    line.append(match)

                if remaining_chars > 0:
                    line.extend(get_chars(ALPHABETS[a_size], remaining_chars))

                f.write("".join(line)[:line_size])
                f.write("\n")


if __name__ == "__main__":
    main(docopt(__doc__))
