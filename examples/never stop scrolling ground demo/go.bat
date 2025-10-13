@echo off
convimg --input convimg.yaml    
move *.c .\src\ 2>nul
move *.h .\src\ 2>nul
make clean
make