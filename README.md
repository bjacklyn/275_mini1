# Extract data to git repo
```
tar -xvf <data.tar.gz>
```

# Compile
```
g++ -std=c++20 -Wall -Werror -o parser parser.cpp
```

# Visualize heap size over time using heaptrack (better than valgrind w/ massif IMO)
```
heaptrack ./parser # automatically opens the visualization if heaptrack_gui also installed, otherwise use heaptrack_print
```

# Visualize heap size over time using valgrind
```
valgrind --tool=massif ./parser
massif-visualizer massif.out.1066
```

# Check for memory leaks using valgrind
```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./parser
```

# Viewing size of structs with padding bytes
```
# First recompile with -g3 to add debug symbols needed by pahole
g++ -std=c++20 -Wall -Werror -o parser -g3 parser.cpp

# View members vs. padding
pahole --class_name=Collision ./parser

# pahole can also reorganize the struct for you to minimize padding bytes wasted
pahole --class_name=Collision --show_reorg_steps --reorganize ./parser
```

# TODO look into gperftools https://goog-perftools.sourceforge.net/doc/heap_profiler.html
