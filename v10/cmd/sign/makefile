all: verify resign enroll sign

verify: verify.o notary.o xs.o ix.o
	cc -o verify verify.o notary.o xs.o ix.o -lipc

resign: resign.o notary.o xs.o ix.o
	cc -o resign resign.o notary.o xs.o ix.o -lipc

enroll: enroll.o notary.o xs.o ix.o
	cc -o enroll enroll.o notary.o xs.o ix.o -lipc

sign: sign.o xs.o
	cc -o sign sign.o xs.o
