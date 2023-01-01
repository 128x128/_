import random
import math 

random.seed(1337) 

class Node:
	id   = None
	data = None 
	type = type
	ctx  = None
	# def __init__(self, id=None, data=None, type=None, ctx=None):
		# self.id   = id
		# self.data = data
		# self.type = type
		# self.ctx  = ctx
	

	# def label(self, name): self.id = name; return self
	# def new(id=None, data=None, type=None): n=Node(id=id, data=data, type=type); n.type._ = n; return n
	# def save(self, ctx): ctx.append(self.fn) if (ctx!=None) else None
	# def apply(self, op, x, ctx=None): self.fn = Node.Function(op, x, self); self.save(ctx); return self

	# def Variable(data): return Node.new(id="Variable", data=data, type=Variable())
	# def Function(op, x, y): return Node.new(id="Function", data=(x, y), type=Function(op))
	# def Tensor(shape, data=[]): return Node.new(id="Tensor", data=data, type=Tensor(shape))

	# def ones(*shape):  return Node.Tensor(shape, data=[Node.Variable(1) for _ in range(math.prod(shape))])
	# def zeros(*shape): return Node.Tensor(shape, data=[Node.Variable(0) for _ in range(math.prod(shape))])
	# def randn(*shape): return Node.Tensor(shape, data=[Node.Variable(random.uniform(-1, 1)) for _ in range(math.prod(shape))])

	# def __call__(self): return self.type.__call__();
	# def __str__(self): return self.type.__str__()
	# def __repr__(self): return self.type.__repr__()
	# def __getitem__(self, x): return self.type.__getitem__(x) 

	# def __add__(a, b): return a.type + b.type
	# def __sub__(a, b): return a.type - b.type
	# def __mul__(a, b): return a.type * b.type

class Function(Node):
	def __init__(self, type): 
		self.out = out
	def apply(self):
		return self.out

	# def __str__(self): return f"{self._.id}: {self._.data[0]} -> {self._.data[1]}\n"
	# def __repr__(self): return f"{self.op.__name__}{self._.data[0]} -> {self._.data[1]}\n";
	# def __call__(self): return self.op(self._.data[0], self._.data[1])

	# def varadd(x, y): y.data = x[0].data + x[1].data
	# def varsub(x, y): y.data = x[0].data - x[1].data
	# def varmul(x, y): y.data = x[0].data * x[1].data
		
	# def sum(x, y): y.data = sum([_.data for _ in x])

# class Binary(Function):
	# def __init__(self, x, y, z): 
		# self.x = x
		# self.y = y
		# self.z = z

# class Add(Binary): 
	# def __init__(self, x, y): 

class Variable(Node): 
	def __init__(self, data): 
		self.data = data

	def __str__(self): return f"{self.id} = {self.__repr__()}" if self.id!=None else f"{self.__repr__()}" 
	def __repr__(self): x=f"{format(self.data,'.4f')}"; return "+"+x if self.data>0 else x;
	# def __str__(self): return f"{self._.id} = {self.__repr__()}" if self._.id!="Variable" else f"{self.__repr__()}" 
	# def __repr__(self): x=f"{format(self._.data,'.4f')}"; return " "+x if self._.data>0 else x;
	# def __getitem__(self, x): return x
	# def __add__(a, b): return Node.Variable(data=a._.data+b._.data).apply(Function.varadd, (a._, b._), ctx=STACK)
	# def __sub__(a, b): return Node.Variable(data=a._.data-b._.data).apply(Function.varsub, (a._, b._), ctx=STACK)
	# def __mul__(a, b): return Node.Variable(data=a._.data*b._.data).apply(Function.varmal, (a._, b._), ctx=STACK)

x = Variable(1)
print(x)
# y = Variable(2)
# z = Add(x, y)
# print(x)
	

# class Tensor: 
	# def __init__(self, shape): self.shape = self.reshape(shape)
	# def nmat(self): return math.prod(self.shape[0:-2])
	# def nvec(self): return math.prod(self.shape[0:-1])
	# def mxn(self): return self.shape[-2]*self.shape[-1]
	# def idx(self, x): return x if isinstance(x, int) else sum([x[-(_+1)]*self.stride[-(_+1)] for _ in range(len(x))])

	# def vec(self, i): return [self._.data[_+i*self.shape[-1]] for _ in range(self.shape[-1])]
	# def mat(self, i): return [self.vec(i*self.shape[-2]+_) for _ in range(self.shape[-2])]

	# def indent(self, v, i): s=self._.id+str(self.shape)+":"; v.insert(0, s) if i == 0 else v.insert(0, len(s)*" "); return v
	# def vecstr(self, i): return " ".join( self.indent([self._.data[_+i*self.shape[-1]].__repr__() for _ in range(self.shape[-1])], i) )
	# def matstr(self, i): return "\n".join( [self.vecstr(i*self.shape[-2]+_) for _ in range(self.shape[-2])] )

	# def __str__(self): return "\n\n".join([self.matstr(_) for _ in range(self.nmat())])+"\n" 
	# def __repr__(self): return self._.id + str(self.shape);
	# def __getitem__(self, x): return self._.data[x] if isinstance(x, int) else self._.data[sum([x[-(_+1)]*self.stride[-(_+1)] for _ in range(len(x))])]

	# def restride(self): return tuple([math.prod(self.shape[_+1:len(self.shape)]) for _ in range(len(self.shape))])
	# def reshape(self, shape): self.shape = shape; self.stride=self.restride(); return self.shape
	# def transpose(self): s=list(self.shape); s[-1]=self.shape[-2]; s[-2]=self.shape[-1]; return self.reshape(tuple(s))
	# def flatten(self): return self.reshape((math.prod(self.shape), 1)) 
	
	# def mulshape(a, b): _=list(a.shape[0:-2]); _.append(a.shape[-2]); _.append(b.shape[-1]); return tuple(_)
	# # def matmul(self, v, a, b):
		# # m,k,n = a.type.shape[-1], b.type.shape[-2], b.type.shape[-1] 
		# # for i in range(m):
			# # for j in range(n):
				# # self._.data[self.idx(v, i, j)] = 
		# # return self

	# def tenmul(a, b): return [sum([a._.data[0]*a._.data[0] for _ in range(a.shape)]) for _ in range(math.prod(a.mulshape(b)))]

	# def __add__(a, b): return Node.new(id=f"({a._.id}+{b._.id})",type=Tensor(a.shape),data=[a[_]+b[_] for _ in range(len(a._.data))]).apply(None,(a, b))
	# def __sub__(a, b): return Node.new(id=f"({a._.id}-{b._.id})",type=Tensor(a.shape),data=[a[_]-b[_] for _ in range(len(a._.data))]).apply(None,(a, b))
	# def __mul__(a, b): return Node.new(id=f"({a._.id}x{b._.id})",type=Tensor(a.mulshape(b)),data=[a.tenmul(b)]).apply(None,(a, b))
