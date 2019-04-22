com = g++
program = test

deps = $(powershell gci . -name "*.h")
src = $(powershell gci . -name "*.cpp")
obj = $(src:%.cpp=%.o)

clean:
	del  *.txt $(obj) $(program)

echo1:
	echo $(src)

$(program): $(obj)
	$(com) -o $(program) $(obj)

%.o : $(deps)
	$(com) -c $< -o $@

	