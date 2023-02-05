#!/bin/bash

rmmod kernel_nl
make
insmod kernel_nl.ko

make client