#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Script to generate plots with the results of string searching algorithms.

<input_folder> The folder where the results can be found.

Usage:
    plot_gen.py <input_folder>
"""

import csv
import os
import re
from statistics import mean

from docopt import docopt
import plotly.plotly as py
import plotly.graph_objs as go


DATASET_REGEX = re.compile(r'(?P<line_size>[0-9]+)_ls')


def main(args):
    input_folder = args['<input_folder>']

    # indexing
    indexing_data = {}
    indexing_layout = go.Layout(title="Indexing", width=800, height=640,
                                xaxis=dict(title='Tamanho do Texto (characteres)'),
                                yaxis=dict(title='Tempo de Execução (milisegundos)'))

    # searching
    searching_data = {}
    searching_layout = go.Layout(title="Searching", width=800, height=640,
                                 xaxis=dict(title='Tamanho do Texto (characteres)'),
                                 yaxis=dict(title='Tempo de Execução (milisegundos)'))

    for dataset in os.listdir(input_folder):
        dataset_path = os.path.join(input_folder, dataset)
        line_size = DATASET_REGEX.match(dataset).groupdict()["line_size"]

        # indexing
        with open(os.path.join(dataset_path, 'indexing_runtime.csv')) as csvfile:
            runtime = csv.DictReader(csvfile, delimiter=',')
            indexing_data[line_size] = mean([int(row["runtime"]) for row in runtime]) / 1000000

        # indexing
        with open(os.path.join(dataset_path, 'searching_runtime.csv')) as csvfile:
            runtime = csv.DictReader(csvfile, delimiter=',')
            searching_data[line_size] = mean([int(row["runtime"]) for row in runtime]) / 1000000

    indexing_data = sorted(list(indexing_data.items()))
    searching_data = sorted(list(searching_data.items()))

    os.makedirs("plots", exist_ok=True)

    figure = go.Figure(data=[go.Scatter(x=[data[0] for data in indexing_data], y=[data[1] for data in indexing_data], mode='lines', name="Indexing")],
                                        layout=indexing_layout)
    py.image.save_as(figure, filename='plots/indexing.png')

    figure = go.Figure(data=[go.Scatter(x=[data[0] for data in searching_data], y=[data[1] for data in searching_data], mode='lines', name="Searching")],
                                        layout=searching_layout)
    py.image.save_as(figure, filename='plots/searching.png')


if __name__ == "__main__":
    main(docopt(__doc__))
