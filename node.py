import random
import math 

random.seed(1337) 

ALL = []

class Variable: 
	def __init__(self): self._ = None
	def __str__(self): return f"{self._.id} = {self.__repr__()}" 
	def __repr__(self): x=f"{format(self._.data,'.4f')}"; return " "+x if self._.data>0 else x;

class Node:

	def __init__(self, id="Variable", data=0, type=Variable()):
		self.id   = id
		self.data = data
		self.type = type
		ALL.append(self)

	def new(id="Variable", data=0, type=Variable()): n=Node(id=id, data=data, type=type); n.type._=n; return n
	# def rand(a=-1,b=1): return Node.new(data=random.uniform(a, b))
	def Tensor(*shape): return Node.new(id="Tensor", data=[Node.new() for _ in range(math.prod(shape))], type=Tensor(shape))

	def __str__(self): return self.type.__str__()
	def __repr__(self): return self.type.__repr__()

class Function:
	def __init__(self, id=None, type=None):
		self.id = id

class Unary:
	def __init__(self, x, y):
		self.x = x
		self.y = y
class Binary:
	def __init__(self, x, y, z):
		self.x = x
		self.y = y
		self.z = z
class Reduce:
	def __init__(self, x, y):
		self.x = x
		self.y = y


class Tensor: 
	def __init__(self, shape): self._ = None; self.shape = shape 
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

	def reshape(self, *shape): self.shape = shape; return self.shape
	def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return tuple(s)
	def flatten(self): self.shape = (math.prod(self.shape), 1); return self.shape


# x = Node.new()
# print(x)
x = Node.Tensor(4, 2, 3)
print(x)
