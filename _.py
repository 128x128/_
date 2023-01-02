import random
import math 
from typing import List, Tuple, Callable, Optional

# from rich import print, pretty

random.seed(1337) 

STACK = []

class Node:
	id   = None
	data = None
	type = type
	ctx  = None

	# printer
	def indent(self, buffer:str, n:int): return buffer.replace("\n", "\n"+" "*n)
	def label(self, buffer:list): s=self.id+" = "; buffer.insert(0, s); return self.indent("".join(buffer), len(s))
	def colorise(self, buffer:str): return f"\033[95m{buffer}\033[0m"
	def format(self, buffer:list): return "".join(buffer) if self.id==None else self.colorise(self.label(buffer))
	
	def push(self, t, e): temp = list(t); temp.append(e); return tuple(temp)

	# BINARY OPS
	def __add__(x, y):     return x.add(y)
	def __sub__(x, y):     return x.sub(y)
	def __mul__(x, y):     return x.mul(y) 
	def __truediv__(x, y): return x.div(y) 


class Function(Node):
	def __init__(self, *fxn, op=None): 
		self.data = fxn 
		self.op = op
	@property
	def ret(self): return self.data[-1]
	def apply(self, ret): 
		self.data = self.push(self.data, ret) 
		self.ret.ctx = self
		if isinstance(self.ret, Tensor): self.ret.permuate(self)
		if isinstance(self.ret, Variable): self.__call__()
		return self.ret
	def __call__(self): 
		self.op(self.data) # invoke function
	def __str__(self):
		return f"F: {self.data}"

class Variable(Node):
	def __init__(self, data=0): self.data = data; self.type = Variable
	def __str__(self): return f"{self.id} = {self.__repr__()}" if self.id!=None else f"{self.__repr__()}" 
	def __clr__(self, x:str): return f"\033[92m+{x}\033[0m" if self.data>0 else f"\033[91m{x}\033[0m"
	def __repr__(self): return self.__clr__(f"{format(self.data,'.4f')}")
	def __call__(self): self.ctx(); return self


	# unary
	def exp(self): return None
	def relu(self): return None
	def sigmoid(self): return None

	# binary 
	def cadd(_): _[-1].data=_[0].data+_[1].data
	def csub(_): _[-1].data=_[0].data-_[1].data
	def cmul(_): _[-1].data=_[0].data*_[1].data
	def cdiv(_): _[-1].data=_[0].data/_[1].data
	def add(x, y): return Function(x, y, op=Variable.cadd).apply(Variable())
	def sub(x, y): return Function(x, y, op=Variable.csub).apply(Variable())
	def mul(x, y): return Function(x, y, op=Variable.cmul).apply(Variable())
	def div(x, y): return Function(x, y, op=Variable.cdiv).apply(Variable())

	def csum(_): _[-1].data=sum([x.data for x in _[0].data])
	def cmax(_): _[-1].data=max([x.data for x in _[0].data])
	def cmin(_): _[-1].data=min([x.data for x in _[0].data])

class Tensor(Node):
	def __init__(self, shape, data=None): self.data = data; self.shape = self.reshape(shape)
	# initializers
	def empty(*shape):  return Tensor(shape, data=[])
	def fill(*shape):  return Tensor(shape, data=[None  for _ in range(math.prod(shape))])
	def ones(*shape):  return Tensor(shape, data=[Variable(1) for _ in range(math.prod(shape))])
	def zeros(*shape): return Tensor(shape, data=[Variable(0) for _ in range(math.prod(shape))])
	def randn(*shape): return Tensor(shape, data=[Variable(random.uniform(-1, 1)) for _ in range(math.prod(shape))])
	def arange(*shape): return Tensor(shape, data=[Variable(_+1) for _ in range(math.prod(shape))])

	# tensor shape ops
	def restride(self): return tuple([math.prod(self.shape[_+1:len(self.shape)]) for _ in range(len(self.shape))])
	def reshape(self, shape): self.shape = shape; self.stride=self.restride(); return self.shape
	def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return self.reshape(tuple(s))
	def flatten(self): return self.reshape((math.prod(self.shape), 1)) 

	def nmat(self): return math.prod(self.shape[0:-2])
	def nvec(self): return math.prod(self.shape[0:-1])
	def mxn(self): return self.shape[-2]*self.shape[-1]
	def idx(self, *i): return i if isinstance(i, int) else sum([i[-(_+1)]*self.stride[-(_+1)] for _ in range(len(i))])

	def matidx(self, i:int): return self.mxn()*i
	def vec(self, i:int): return [self.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	def mat(self, i:int): return [self.vec(i*self.shape[-2]+_) for _ in range(self.shape[-2])]
	def matrow(self, v:int, i:int): return [self.data[self.matidx(v)+(i*self.shape[-1])+_] for _ in range(self.shape[-1]) ]
	def matcol(self, v:int, i:int): return [self.data[self.matidx(v)+(self.shape[-1]*_)+i] for _ in range(self.shape[-2]) ]

	def vecstr(self, i:int): return " ".join( [self.data[_+i*self.shape[-1]].__repr__() for _ in range(self.shape[-1])])
	def matstr(self, i:int): return "\n".join( [self.vecstr(i*self.shape[-2]+_) for _ in range(self.shape[-2])])

	def __str__(self): return self.format(list("\n\n".join([self.matstr(_) for _ in range(self.nmat())])+"\n"))
	def __repr__(self): return self.id + str(self.shape);
	def __getitem__(self, i): return self.data[self.idx(i)]

	# tensor compute ops
	def broadcast(self, data):
		for _ in range(len(data)): self.data[_] = data[_]
	def permuate(self, fn=None):
		if (fn.op==Variable.cadd): self.broadcast([fn.data[0].data[_]+fn.data[1].data[_] for _ in range(len(self.data))])
		if (fn.op==Variable.csub): self.broadcast([fn.data[0].data[_]-fn.data[1].data[_] for _ in range(len(self.data))])
		if (fn.op==Variable.cmul): self.tenmul(fn.data[0], fn.data[1])
	def __call__(x): 
		for _ in x[-1].data: _()

	# UNARY OPS
	def softmax(self): return None

	# BINARY OPS
	def add(x, y): return Function(x, y, op=Variable.cadd).apply(Tensor.fill(*x.shape))
	def sub(x, y): return Function(x, y, op=Variable.csub).apply(Tensor.fill(*x.shape))
	def mul(x, y): return Function(x, y, op=Variable.cmul).apply(Tensor.fill(*x.mulshape(y)))

	def mulshape(x, y): _=list(x.shape[0:-2]); _.append(x.shape[-2]); _.append(y.shape[-1]); return tuple(_)
	def matmul(self, x, y, _):
		for i in range(x.shape[-2]):
			for j in range(y.shape[-1]):
				T = Tensor.fill(y.shape[-2], 1);
				T.broadcast([v[0]*v[1] for v in zip(x.matrow(_, i), y.matcol(_, j))])	
				if len(self.shape)==2: self.data[self.idx(i,j)] = T.sum()  
				else: self.data[self.idx(_,i,j)] = T.sum() 
	def tenmul(self, x, y):
		for _ in range(self.nmat()): self.matmul(x, y, _)

	# REDUCE OPS
	def sum(self): return Function(self, op=Variable.csum).apply(Variable())
	def min(self): return Function(self, op=Variable.cmin).apply(Variable())
	def max(self): return Function(self, op=Variable.cmax).apply(Variable())





x = Tensor.randn(3, 4)
y = Tensor.randn(4, 3)

z = x * y

x.id = "x"
y.id = "y"
z.id = "z"

print(x)
print(y)
print(z)
