include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	 $ cat <movie7 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean