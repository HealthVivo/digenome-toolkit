from sys import argv # [-p prefix], [-d direction], 1_freq, 1_depth, 2_freq, 2_depth, ...
from os.path import isfile
prefix = ""
direction = ""

fns = []
for i in range(1, len(argv)):
    if i != len(argv)-1 and argv[i] == "-p":
        prefix = argv[i+1]
    elif i != len(argv)-1 and argv[i] == "-d":
        direction = argv[i+1]
    elif i != 1 and argv[i-1] != "-p" and argv[i-1] != "-d":
        fns.append(argv[i])

if direction != "":
    fns = zip(fns[::2], fns[1::2])
    if prefix == "":
        fo = open("%s.txt"%direction, "a")
    else:
        fo = open("%s_%s.txt"%(prefix, direction), "a")

    for ffn, fdn in fns:
        print ("Processing {0} and {1}...".format(ffn, fdn))
        ff = open(ffn)
        fd = open(fdn)
        for ff_line in ff:
            pos = ff_line.split('\t')[0]
            while True:
                fd_line = fd.readline()
                try:
                    if fd_line.split('\t')[0].split(':')[1] == pos:
                        fo.write(fd_line)
                        break
                except:
                    print (ff_line, fd_line)
                    break
        ff.close()
    fo.write("\n")
