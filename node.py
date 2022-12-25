import random
import math 

random.seed(1337) 

ALL = []

class Node:

	def __init__(self, id=None, data=0, meta=[], type=None):
		self.id   = id
		self.data = data  
		self.type = type
		for x in meta: setattr(self, x[0], x[1])
		ALL.append(self)

	def istype(t): return self.type==t
	def rand(a=-1,b=1): return Node(data=random.uniform(a, b))

	def Tensor(*shape): return Node(id="Tensor", data=[Node.rand() for _ in range(math.prod(shape))], meta=[('shape', shape)], type=Tensor)

	def __str__(self):
		if (self.type!=None): return self.type.str(self)
		return f"+{format(self.data, '.4f')}" if self.data>0 else f"{format(self.data, '.4f')}" 

	def __repr__(self):
		if (self.type!=None): return self.type.str(self)
		return f"+{format(self.data, '.4f')}" if self.data>0 else f"{format(self.data, '.4f')}" 

class Function:
	def __init__(self, id=None, out=None):
		self.id = out

class Tensor: 
	  def nmat(self): return math.prod(self.shape[0:-2])
	  def nvec(self): return math.prod(self.shape[0:-1])
	  def mxn(self): return self.shape[-1]*self.shape[-2]
	  def vec(self, i): return [self.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	  def mat(self, i): return [Tensor.vec(self, i*self.shape[-2]+_) for _ in range(self.shape[-2])]
	  def indent(self, v, i): s=self.id+":"; v.insert(0, s) if i == 0 else v.insert(0, len(s)*" "); return v
	  def vecstr(self, i): return " ".join( Tensor.indent(self, [str(self.data[_+i*self.shape[-1]]) for _ in range(self.shape[-1])], i) )
	  def matstr(self, i): return "\n".join( [Tensor.vecstr(self, i*self.shape[-2]+_) for _ in range(self.shape[-2])] )
	  def str(self): return "\n\n".join([Tensor.matstr(self, _) for _ in range(Tensor.nmat(self))]) 
			   


x = Node.Tensor(5, 32, 16)
print(x)

