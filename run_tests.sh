#!/usr/bin/env bash

DATA_FOLDER="datasets"
OUTPUT_FOLDER="test_results"


for dataset in $DATA_FOLDER/*; do
	dataset_name=`basename "$dataset"`
	outpur_folder="$OUTPUT_FOLDER/$dataset_name/"
	mkdir -p "$outpur_folder"

	echo "Dataset: ${dataset_name}"

	echo "Indexing ..."
	./bin/ipmt --report-runtime index $dataset/txt/text0.txt &> output.txt
	mv runtime.csv "$outpur_folder/indexing_runtime.csv"
	mv output.txt "$outpur_folder/indexing_output.txt"

	echo "Searching ..."
	./bin/ipmt --count --report-runtime --pattern "$dataset/patterns.txt" search text0.idx &> output.txt
	mv runtime.csv "$outpur_folder/searching_runtime.csv"
	mv output.txt "$outpur_folder/searching_output.txt"

	rm text0.idx
	
done
