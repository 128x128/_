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
	def format(self, buffer:list): return "".join(buffer) if self.id==None else self.label(buffer)
	
	def push(self, t, e): temp = list(t); temp.append(e); return tuple(temp)

	# BINARY OPS
	def __add__(x, y):     return x.add(y)
	def __sub__(x, y):     return x.sub(y)
	def __mul__(x, y):     return x.mul(y) 
	def __truediv__(x, y): return x.div(y) 


class Function(Node):
	def __init__(self, *fxn, op=None): 
		self.data = self.push(fxn,fxn[0].type()) 
		self.op = op
		self.__call__()
	@property
	def ret(self): return self.data[-1]
	def __call__(self): self.op(self.data)

class Variable(Node):
	def __init__(self, data=0): self.data = data; self.type = Variable
	def __str__(self): return f"{self.id} = {self.__repr__()}" if self.id!=None else f"{self.__repr__()}" 
	def __repr__(self): x=f"{format(self.data,'.4f')}"; return "+"+x if self.data>0 else x;
	def __call__(self): self.ctx(); return self

	@staticmethod
	def cadd(_): _[-1].data=_[0].data+_[1].data
	@staticmethod
	def csub(_): _[-1].data=_[0].data-_[1].data
	@staticmethod
	def cmul(_): _[-1].data=_[0].data*_[1].data
	@staticmethod
	def cdiv(_): _[-1].data=_[0].data/_[1].data
	def add(x:Variable, y:Variable): return Function(x, y, op=Variable.cadd).ret
	def sub(x:Variable, y:Variable): return Function(x, y, op=Variable.csub)
	def mul(x:Variable, y:Variable): return Function(x, y, op=Variable.cmul)
	def div(x:Variable, y:Variable): return Function(x, y, op=Variable.cdiv)

class Tensor(Node):
	def __init__(self, shape, data=None): 
		self.data = data
		self.shape = self.reshape(shape)

	# Initializers
	@staticmethod
	def ones(*shape):  return Tensor(shape, data=[Variable(1) for _ in range(math.prod(shape))])
	@staticmethod
	def zeros(*shape): return Tensor(shape, data=[Variable(0) for _ in range(math.prod(shape))])
	@staticmethod
	def randn(*shape): return Tensor(shape, data=[Variable(random.uniform(-1, 1)) for _ in range(math.prod(shape))])

	# tensor shape ops
	def restride(self): return tuple([math.prod(self.shape[_+1:len(self.shape)]) for _ in range(len(self.shape))])
	def reshape(self, shape): self.shape = shape; self.stride=self.restride(); return self.shape
	def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return self.reshape(tuple(s))
	def flatten(self): return self.reshape((math.prod(self.shape), 1)) 

	def nmat(self): return math.prod(self.shape[0:-2])
	def nvec(self): return math.prod(self.shape[0:-1])
	def mxn(self): return self.shape[-2]*self.shape[-1]
	def idx(self, x): return x if isinstance(x, int) else sum([x[-(_+1)]*self.stride[-(_+1)] for _ in range(len(x))])

	def vec(self, i): return [self.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	def mat(self, i): return [self.vec(i*self.shape[-2]+_) for _ in range(self.shape[-2])]

	def vecstr(self, i:int): return " ".join( [self.data[_+i*self.shape[-1]].__repr__() for _ in range(self.shape[-1])])
	def matstr(self, i:int): return "\n".join( [self.vecstr(i*self.shape[-2]+_) for _ in range(self.shape[-2])])

	def __str__(self): return self.format(list("\n\n".join([self.matstr(_) for _ in range(self.nmat())])+"\n"))
	def __repr__(self): return self.id + str(self.shape);

	# UNARY OPS
	def relu(self): return None
	def exp(self): return None
	def softmax(self): return None
	def sigmoid(self): return None

	# BINARY OPS
	def add(self): return None
	def sub(self): return None
	def mul(self): return None

	# REDUCE OPS
	def sum(self): return None
	def min(self): return None
	def max(self): return None

	# def __getitem__(self, x): return self._.data[x] if isinstance(x, int) else self._.data[sum([x[-(_+1)]*self.stride[-(_+1)] for _ in range(len(x))])]

	
	# def mulshape(a, b): _=list(a.shape[0:-2]); _.append(a.shape[-2]); _.append(b.shape[-1]); return tuple(_)
	# def matmul(self, v, a, b):
		# m,k,n = a.type.shape[-1], b.type.shape[-2], b.type.shape[-1] 
		# for i in range(m):
			# for j in range(n):
				# self._.data[self.idx(v, i, j)] = 
		# return self

	# def tenmul(a, b): return [sum([a._.data[0]*a._.data[0] for _ in range(a.shape)]) for _ in range(math.prod(a.mulshape(b)))]

x = Variable(1)
y = Variable(3)
z = x + y
print(z)

