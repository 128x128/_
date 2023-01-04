# import random
import math, random, statistics
from typing import List, Tuple, Callable, Optional

random.seed(1337) 

class Node:
	id   = None
	data = None
	type = type
	ctx  = None

	# output format
	def indent(self, buffer:str, n:int): return buffer.replace("\n", "\n"+" "*n)
	def label(self, buffer:list): s=self.id+" = "; buffer.insert(0, s); return self.indent("".join(buffer), len(s))
	def colorise(self, buffer:str): return f"\033[95m{buffer}\033[0m"
	def format(self, buffer:list): return "".join(buffer) if self.id==None else self.colorise(self.label(buffer))

	# helper methods
	def push(self, t, e): temp = list(t); temp.append(e); return tuple(temp)

	# ops
	def __neg__(x):        return x.neg()
	def __add__(x, y):     return x.add(y)
	def __sub__(x, y):     return x.sub(y)
	def __mul__(x, y):     return x.mul(y) 
	def __truediv__(x, y): return x.div(y) 


class Function(Node):
	def __init__(self, *fxn, op=None): self.data = fxn; self.op = op
	@property
	def ret(self): return self.data[-1]
	@property
	def name(self): return f"\033[96m{self.op.__name__}\033[0m"
	def apply(self, ret): 
		self.data = self.push(self.data, ret) 
		self.ret.ctx = self
		if isinstance(self.ret, Tensor): self.ret.permuate(op=self.op, x=self.data[0], y=self.data[1])
		if isinstance(self.ret, Variable): self.__call__()
		return self.ret
	def __call__(self): 
		self.op(self.data)
		# print(self.__repr__())
	
	def __repr__(self): 
		if (len(self.data)==2): return f"{self.name} ( {self.data[0].id} ) -> {self.data[1].id}"
		if (len(self.data)==3): return f"{self.name} ( {self.data[0].id} {self.data[1].id} ) -> {self.data[2].id}"
	def __str__(self): 
		if (len(self.data)==2): return f"{self.name}( x={self.data[0].id} ):\n{self.data[0]} \n= {self.data[1]}\n"
		if (len(self.data)==3): return f"{self.name}( x={self.data[0].id}, y={self.data[1].id} ):\n{self.data[0]}, \n{self.data[1]} \n= {self.data[2]}\n"

class Variable(Node):
	def __init__(self, data=0): self.data = data; self.type = Variable
	def __str__(self): return f"{self.id} = {self.__repr__()}" if self.id!=None else f"{self.__repr__()}" 
	def __clr__(self, x:str): return f"\033[92m+{x}\033[0m" if self.data>0 else f"\033[91m{x}\033[0m"
	def __repr__(self): return self.__clr__(f"{format(self.data,'.4f')}")
	def __call__(self): self.ctx(); return self


	# Unary: (Variable) -> Variable
	def cneg(_): _[-1].data=_[0].data*-1
	def cexp(_): _[-1].data=math.exp(_[0].data)
	def csqrt(_): _[-1].data=math.sqrt(_[0].data)
	def crelu(_): _[-1].data=math.exp(_[0].data)
	def csigmoid(_): _[-1].data=math.exp(_[0].data)

	def neg(self): return Function(x,op=Variable.cneg).apply(Variable())
	def exp(self): return Function(x,op=Variable.cexp).apply(Variable())
	def sqrt(self): return Function(x,op=Variable.csqrt).apply(Variable())
	def relu(self): return Function(x,op=Variable.crelu).apply(Variable())
	def sigmoid(self): return Function(x,op=Variable.csigmoid).apply(Variable())

	# Binary: (Variable x Variable) -> Variable
	def cadd(_): _[-1].data=_[0].data+_[1].data
	def csub(_): _[-1].data=_[0].data-_[1].data
	def cmul(_): _[-1].data=_[0].data*_[1].data
	def cdiv(_): _[-1].data=_[0].data/_[1].data
	def cpow(_): _[-1].data=math.pow(_[0].data, _[1].data)

	def add(x, y): return Function(x, y, op=Variable.cadd).apply(Variable())
	def sub(x, y): return Function(x, y, op=Variable.csub).apply(Variable())
	def mul(x, y): return Function(x, y, op=Variable.cmul).apply(Variable())
	def div(x, y): return Function(x, y, op=Variable.cdiv).apply(Variable())
	def pow(x, y): return Function(x, y, op=Variable.cpow).apply(Variable())

	# Reduce: Tensor -> Variable
	def csum(_): _[-1].data=sum([x.data for x in _[0].data])
	def cmax(_): _[-1].data=max([x.data for x in _[0].data])
	def cmin(_): _[-1].data=min([x.data for x in _[0].data])
	def cavg(_): _[-1].data=statistics.avg([x.data for x in _[0].data])
	def cmean(_): _[-1].data=statistics.mean([x.data for x in _[0].data])


	

class Tensor(Node):
	def __init__(self, shape, data=None, dtype=None): self.data = data; self.shape = self.reshape(shape); self.dtype=dtype

	# initializers
	@staticmethod
	def empty(*shape): return Tensor(shape, data=[], dtype=None)
	@staticmethod
	def fill(*shape):  return Tensor(shape, data=[None  for _ in range(math.prod(shape))], dtype=Variable)
	@staticmethod
	def ones(*shape):  return Tensor(shape, data=[Variable(1) for _ in range(math.prod(shape))], dtype=Variable)
	@staticmethod
	def zeros(*shape): return Tensor(shape, data=[Variable(0) for _ in range(math.prod(shape))], dtype=Variable)
	@staticmethod
	def randn(*shape): return Tensor(shape, data=[Variable(random.uniform(-1, 1)) for _ in range(math.prod(shape))], dtype=Variable)
	@staticmethod
	def arange(*shape): return Tensor(shape, data=[Variable(_+1) for _ in range(math.prod(shape))], dtype=Variable)
	@staticmethod
	def strbuff(*shape): return Tensor(shape, data=["+" for _ in range(math.prod(shape))], dtype=str).inter_replace(shape[1], "\n")

	# tensor shape ops
	def restride(self): return tuple([math.prod(self.shape[_+1:len(self.shape)]) for _ in range(self.rank)])
	def reshape(self, shape): self.shape = shape; self.stride=self.restride(); return self.shape
	def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return self.reshape(tuple(s))
	def flatten(self): return self.reshape((math.prod(self.shape), 1)) 

	@property
	def nmat(self): return math.prod(self.shape[0:-2])
	def nvec(self): return math.prod(self.shape[0:-1])
	def mxn(self): return self.shape[-2]*self.shape[-1]
	def idx(self, *i): return i if isinstance(i, int) else sum([i[-(_+1)]*self.stride[-(_+1)] for _ in range(len(i))])
	def invidx(self, i:int): idx=[]; exec("for s in self.stride: r=i%s;d=i-r;idx.append(int(d/s));i=r"); return tuple(idx)

	def matidx(self, i:int): return self.mxn()*i
	def vec(self, i:int): return [self.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	def mat(self, i:int): return [self.vec(i*self.shape[-2]+_) for _ in range(self.shape[-2])]
	def matrow(self, v:int, i:int): return [self.data[self.matidx(v)+(i*self.shape[-1])+_] for _ in range(self.shape[-1])]
	def matcol(self, v:int, i:int): return [self.data[self.matidx(v)+(self.shape[-1]*_)+i] for _ in range(self.shape[-2])]

	def vecstr(self, i:int): return " ".join( [self.data[_+i*self.shape[-1]].__repr__() for _ in range(self.shape[-1])])
	def matstr(self, i:int): return "\n".join( [self.vecstr(i*self.shape[-2]+_) for _ in range(self.shape[-2])])
	def vecuid(self, i:int): return " ".join( [self.data[_+i*self.shape[-1]].id for _ in range(self.shape[-1])])
	def matuid(self, i:int): return "\n".join( [self.vecuid(i*self.shape[-2]+_) for _ in range(self.shape[-2])])

	def __uid__(self): return self.format(list("\n\n".join([self.matuid(_) for _ in range(self.nmat)])+"\n"))
	def __str__(self): 
		if self.dtype==str: return "".join([_ for _ in self.data])
		if self.dtype==Variable: return self.format(list("\n\n".join([self.matstr(_) for _ in range(self.nmat)])+"\n"))
	def __repr__(self): return self.id + str(self.shape);
	def __getitem__(self, i): return self.data[self.idx(i)]

	# tensor helpers
	@property
	def fn(self): return [_.ctx for _ in self.data]
	@property
	def fnstr(self): return "\n\n".join([_.ctx.__str__() for _ in self.data])
	@property
	def size(self): return len(self.data)
	@property
	def rank(self): return len(self.shape)
	def relabel(self):
		for _ in range(self.size):
			self.data[_].id = f"{self.id}{list(self.inverseidx(_))}:[{_}]"

	# tensor compute ops
	def cast(self, data):
		for _ in range(len(data)): self.data[_] = data[_]
	def permuate(self, op=None, x=None, y=None):
		if (op==Variable.cadd): self.data = [x.data[_]+y.data[_] for _ in range(self.size)]
		if (op==Variable.csub): self.data = [x.data[_]-y.data[_] for _ in range(self.size)]
		if (op==Variable.cmul): self.tenmul(x, y)
	def __call__(self): 
		for _ in self.data: _()

	# UNARY OPS
	def neg(self): return None
	def softmax(self): return None
	def logsoftmax(self): return None
	def conv2d(self): return None
	def avgpool2d(self): return None
	def maxpool2d(self): return None

	# BINARY OPS
	def add(x, y): return Function(x, y, op=Variable.cadd).apply(Tensor.fill(*x.shape))
	def sub(x, y): return Function(x, y, op=Variable.csub).apply(Tensor.fill(*x.shape))
	def mul(x, y): return Function(x, y, op=Variable.cmul).apply(Tensor.fill(*x.mulshape(y)))

	def mulshape(x, y): _=list(x.shape[0:-2]); _.append(x.shape[-2]); _.append(y.shape[-1]); return tuple(_)
	def matmul(self, x, y, _):
		for i in range(x.shape[-2]):
			for j in range(y.shape[-1]):
				T = Tensor.fill(y.shape[-2], 1);
				T.id = f"matmulKernal{[i, j]}"
				T.cast([v[0]*v[1] for v in zip(x.matrow(_, i), y.matcol(_, j))])
				if len(self.shape)==2: self.data[self.idx(i,j)] = T.sum()  
				else: self.data[self.idx(_,i,j)] = T.sum() 
	def tenmul(self, x, y):
		for _ in range(self.nmat): self.matmul(x, y, _)

	# REDUCE OPS
	def sum(self): return Function(self, op=Variable.csum).apply(Variable())
	def min(self): return Function(self, op=Variable.cmin).apply(Variable())
	def max(self): return Function(self, op=Variable.cmax).apply(Variable())
	def mean(self): return Function(self, op=Variable.cmean).apply(Variable())
	def avg(self): return Function(self, op=Variable.cavg).apply(Variable())

	# str tenor ops
	def red(self, buffer:str): return f"\033[92m{buffer}\033[0m"
	def green(self, buffer:str): return f"\033[91m{buffer}\033[0m"
	def blue(self, buffer:str): return f"\033[94m{buffer}\033[0m"
	
	def charbuff(self, n:int): return [" " for _ in range(n)]
	def replace_char(self, _:int, char:str): self.data[_]=char; return self
	def condition(self, cond:str, op:str): print(0) if exec(cond) else None
		# exec(op) if exec(cond) else None 
	def inter_replace(self, n:int, token:str): s=[self.condition(f"{_}%{n}==0", f"self.replace_char({_}, {token})") for _ in range(self.size)];return self

	def merge(x, y):	
		data = [x.data[_] + y.data[_] for _ in range(min(x.m, y.m))]
		return Buffer(max(x.m, y.m), x.n+y.n, data=data)

	def _indent(self, _:list): _.insert(0, 'x'); return _.pop()
	def op(self, fn): return [fn(_) for _ in self.data]
	def indent(self, n:int): return [self.op(self._indent) for _ in range(n)]   

	def _branch(self, _:list): _.insert(0, '├ '); _.pop()
	def _corner(self, _:list): x[0]="┌"; x[-1]="└"; return x
	def branch(self, i, j): return [self._branch() for _ in range(i, j)]


x = Tensor.strbuff(10, 32)
print(x)










































