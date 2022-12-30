import random
import math 

random.seed(1337) 
STACK = []

class Computation:
	def __init__(self, stack=[]):
		self.stack = stack

class Variable: 
	def __str__(self): return f"{self._.id} = {self.__repr__()}" if self._.id!="Variable" else f"{self.__repr__()}" 
	def __repr__(self): x=f"{format(self._.data,'.4f')}"; return " "+x if self._.data>0 else x;
	def __getitem__(self, x): return x
	def __add__(a, b): return Node.Variable(data=a._.data+b._.data).apply(Function.varadd, (a._, b._), ctx=STACK)

class Node:
	def __init__(self, id=None, data=None, type=None):
		self.id   = id
		self.data = data
		self.type = type
		self.fn   = None

	def label(self, name): self.id = name; return self
	def new(id=None, data=None, type=None): n=Node(id=id, data=data, type=type); n.type._ = n; return n
	def save(self, ctx): ctx.append(self.fn) if (ctx!=None) else None
	def apply(self, op, x, ctx=None): self.fn = Node.Function(op, x, self); self.save(ctx); return self

	def Variable(data): return Node.new(id="Variable", data=data, type=Variable())
	def Function(op, x, y): return Node.new(id="Function", data=(x, y), type=Function(op))
	def Tensor(shape, data=[]): return Node.new(id="Tensor", data=data, type=Tensor(shape))

	def ones(*shape):  return Node.Tensor(shape, data=[Node.Variable(1) for _ in range(math.prod(shape))])
	def zeros(*shape): return Node.Tensor(shape, data=[Node.Variable(0) for _ in range(math.prod(shape))])
	def randn(*shape): return Node.Tensor(shape, data=[Node.Variable(random.uniform(-1, 1)) for _ in range(math.prod(shape))])

	def __call__(self): return self.type.__call__();
	def __str__(self): return self.type.__str__()
	def __repr__(self): return self.type.__repr__()
	def __getitem__(self, x): return self.type.__getitem__(x) 

	def __add__(a, b): return a.type + b.type

class Function:
	def __init__(self, op): self.op = op
	def __str__(self): return f"{self._.id}: {self._.data[0]} -> {self._.data[1]}\n"
	def __repr__(self): return f"{self.op.__name__}{self._.data[0]} -> {self._.data[1]}\n";
	def __call__(self): return self.op(self._.data[0], self._.data[1])

	def varadd(x, y): y.data = x[0].data + x[1].data
		

class Tensor: 
	def __init__(self, shape): self.shape = self.reshape(shape)
	def nmat(self): return math.prod(self.shape[0:-2])
	def nvec(self): return math.prod(self.shape[0:-1])
	def mxn(self): return self.shape[-2]*self.shape[-1]

	def vec(self, i): return [self._.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	def mat(self, i): return [self.vec(i*self.shape[-2]+_) for _ in range(self.shape[-2])]

	def indent(self, v, i): s=self._.id+":"; v.insert(0, s) if i == 0 else v.insert(0, len(s)*" "); return v
	def vecstr(self, i): return " ".join( self.indent([self._.data[_+i*self.shape[-1]].__repr__() for _ in range(self.shape[-1])], i) )
	def matstr(self, i): return "\n".join( [self.vecstr(i*self.shape[-2]+_) for _ in range(self.shape[-2])] )

	def __str__(self): return "\n\n".join([self.matstr(_) for _ in range(self.nmat())])+"\n" 
	def __repr__(self): return self._.id + str(self.shape);
	def __getitem__(self, x): return self._.data[x] if isinstance(x, int) else self._.data[sum([x[-(_+1)]*self.stride[-(_+1)] for _ in range(len(x))])]

	def compstride(self): return tuple([math.prod(self.shape[_+1:len(self.shape)]) for _ in range(len(self.shape))])
	def reshape(self, shape): self.shape = shape; self.stride=self.compstride(); return self.shape
	def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return self.reshape(tuple(s))
	def flatten(self): return self.reshape((math.prod(self.shape), 1)) 

	def __add__(a, b): return Node.new(id=f"({a._.id}+{b._.id})",type=Tensor(a.shape),data=[a[_]+b[_] for _ in range(len(a._.data))]).apply(None,(a, b))

x = Node.randn(4, 2, 3).label("x")
y = Node.randn(4, 2, 3).label("y")
print(x)
print(y)
z = x + y 
print(STACK)

# x = Node.Variable(1)
# y = Node.Variable(3)
# z = x + y
# print(z)
# STACK[0]()
# print(z)
