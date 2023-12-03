CXX = g++-12
CXXFLAGS = -std=gnu++17 -Wall -I/usr/include \
-fsanitize=undefined -fsanitize=shift -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=vla-bound -fsanitize=null -fsanitize=return \
-fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size  -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize=nonnull-attribute \
 -fsanitize=returns-nonnull-attribute -fsanitize=bool  -fsanitize=enum -fsanitize=vptr -fsanitize=pointer-overflow -fsanitize=builtin  -fsanitize=thread  # -fsanitize=leak #   # -fsanitize=pointer-compare  -fsanitize=address
# https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html  # leak and address throwing 2 err 
LIBS = -luv -lPocoFoundation -lPocoNet -lxerces-c 

SRCS = datanamesorcery.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = datanamesorcery
BINDIR = bin

all: $(BINDIR) $(BINDIR)/$(EXEC) clean

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)