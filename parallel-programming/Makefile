com = g++
program = test

deps = $(shell find . -name "*.h")
src = $(shell find . -name "*.cpp")
obj = $(src:%.cpp=%.o)

clean:
	rm -rf *.txt $(obj) $(program)

echo1:
	echo $(src)

$(program): $(obj)
	$(com) -o $(program) $(obj)

%.o : %.c $(deps)
	$(com) -c $< -o $@

	