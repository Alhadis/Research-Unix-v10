#! /bin/sh -
# nightly shell script to send mail for unfinished jobs
cd %SDIR%
for job in Q.*
do
	%LDIR%/genmail $job
done
