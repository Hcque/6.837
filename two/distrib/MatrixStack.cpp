#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
	Matrix4f I = Matrix4f::identity();
	m_matrices.push_back(I);
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
	m_matrices.clear();
	MatrixStack();
}

Matrix4f MatrixStack::top()
{
	// Return the top of the stack
	return m_matrices[m_matrices.size()-1];
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
	m_matrices.push_back(top() * m);
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
	m_matrices.erase(m_matrices.end() - 1);
}
