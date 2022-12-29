import random
import math 

random.seed(1337) 


class Variable: 
	def __str__(self): return f"{self._.id} = {self.__repr__()}" 
	def __repr__(self): x=f"{format(self._.data,'.4f')}"; return " "+x if self._.data>0 else x;
	def __getitem__(self, x): return math.prod(x)
	def __add__(a, b): return Node.Variable(data=a._.data+b._.data).apply(Function.add, (a, b))

class Node:
	def __init__(self, id=None, data=None, type=None):
		self.id   = id
		self.data = data
		self.type = type
		self.fn   = None

	def label(self, name): self.id = name; return self
	def new(id=None, data=None, type=None): n=Node(id=id, data=data, type=type); n.type._ = n; return n
	def apply(self, fn, x): self.fn = Node.Function(fn, x, self); return self


	def Variable(data): return Node.new(id="Variable", data=data, type=Variable())
	def Function(fn, x, y): return Node.new(id="Function", data=[x, y], type=Function(fn))
	def Tensor(shape, data=[]): return Node.new(id="Tensor", data=data, type=Tensor(shape))

	def rand(): return Node.Variable(random.uniform(-1, 1))
	def zero(): return Node.Variable(0)
	def one():  return Node.Variable(1)
	def ones(*shape):  return Node.Tensor(shape, data=[Node.one()  for _ in range(math.prod(shape))])
	def zeros(*shape): return Node.Tensor(shape, data=[Node.zero() for _ in range(math.prod(shape))])
	def randn(*shape): return Node.Tensor(shape, data=[Node.rand() for _ in range(math.prod(shape))])

	def __str__(self): return self.type.__str__()
	def __repr__(self): return self.type.__repr__()
	def __getitem__(self, x): return self.type.__getitem__(x) 

	def __add__(a, b): return a.type + b.type

class Function:
	def __init__(self, fn): self.fn = fn
	def __str__(self): return f"{self._.id}: {self._.data[0]} -> {self._.data[1]}" 
	def apply(self): return self.fn(self._.data[0], self._.data[1]) 
	def add(x, y): y = x[0]._.data + x[1]._.data 

class Tensor: 
	def __init__(self, shape): self.shape = shape 

	def nmat(self): return math.prod(self.shape[0:-2])
	def nvec(self): return math.prod(self.shape[0:-1])
	def mxn(self): return self.shape[-2]*self.shape[-1]

	def vec(self, i): return [self._.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	def mat(self, i): return [self.vec(i*self.shape[-2]+_) for _ in range(self.shape[-2])]

	def indent(self, v, i): s=self._.id+":"; v.insert(0, s) if i == 0 else v.insert(0, len(s)*" "); return v
	def vecstr(self, i): return " ".join( self.indent([self._.data[_+i*self.shape[-1]].__repr__() for _ in range(self.shape[-1])], i) )
	def matstr(self, i): return "\n".join( [self.vecstr(i*self.shape[-2]+_) for _ in range(self.shape[-2])] )

	def __str__(self): return "\n\n".join([self.matstr(_) for _ in range(self.nmat())]) 
	def __repr__(self): return self._.id + str(self.shape);
	def __getitem__(self, x): return sum([math.prod(_) for _ in zip(x, self.shape)])

	def reshape(self, *shape): self.shape = shape; return self.shape
	def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return tuple(s)
	def flatten(self): self.shape = (math.prod(self.shape), 1); return self.shape

	# def matop(self, x, y): 
		# for i in range(self.shape[-2]): 
			# for j in range(self.shape[-1]): 
				
				
	# def matadd(self, a, b, c, i): 
		# for _ in range(len(x._.data)) 
	def __add__(a, b): return Node.new(id="T", type=Tensor(self.shape), data=[]).apply(Function.add, (a, b))
	# for _ in range(self.nmat()): self.matadd(a, b, c, i)

x = Node.ones(2, 3).label("x")
print(x)

# x = Node.rand().label("x")
# y = Node.rand().label("y")  
# z = x + y
# print(z.fn)
