#include "SkeletalModel.h"

#include <FL/Fl.H>
#include <fstream>

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	
	// read lines
	char buffer[128];
	ifstream file(filename);
	int count = 0;
	while (file.getline(buffer, 128)) {
		cout << count << endl;
		stringstream ss(buffer);
		float x, y, z;
		int i;
		ss >> x >> y >> z >> i;
		Joint *joint;
		if (i == -1) {
			m_rootJoint = new Joint;
			joint = m_rootJoint;
		} else {
			joint = new Joint;
		}
		joint->transform = Matrix4f::translation(x, y, z);
		m_joints.push_back(joint);

		if (i != -1) {
			m_joints[count]->children.push_back(m_joints[i]);
		}
		count++;
	}
}

// helper method
void plotJoint(Joint *joint, MatrixStack & matrixStack) {
	// base case 
	if (joint->children.size() == 0) {
		return;
	}

	cout << joint << endl;
	glLoadMatrixf( matrixStack.top());
	glutSolidSphere(0.025f, 12, 12);
	for (int i = 0; i < joint->children.size(); ++i) {
		Matrix4f T = joint->transform;
		matrixStack.push(T);
		plotJoint(joint->children[i], matrixStack);
		matrixStack.pop();
	}
}
void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.

	m_matrixStack = MatrixStack();
	plotJoint(m_rootJoint, m_matrixStack);

}


void plotBones(Joint *joint, Joint *root) {
	if (joint == root) return;
	if (joint->children.size() == 0) return;

	// plot bones
	glutSolidCube( 1.0f );


	for (unsigned i = 0; i < joint->children.size(); ++i) {
		Joint *child = joint->children[i];
		plotBones(child, root);
	}

}
void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	plotBones(m_rootJoint, m_rootJoint);
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
}

