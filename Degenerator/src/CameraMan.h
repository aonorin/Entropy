/*/////////////////////////////////////////////////////////////////////////////////
/// Copyright (c) 2012 Ilija Boshkov
//
/// The MIT License
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////*/

#ifndef __QtCameraMan_H__
#define __QtCameraMan_H__

#include "Ogre.h"
#include <QtGui>
#include <limits>

/// @todo: Implement proper "Fly" mode
/// @todo: Implement the "Orbit" mode

namespace QOgre
{
    enum CameraMode   // enumerator values for different styles of camera movement
    {
	CM_BLENDER,
	CM_FLY,	    // WIP
	CM_ORBIT    // WIP
    };

    enum View
    {
	VI_TOP,
	VI_LEFT,
	VI_BOTTOM,
	VI_RIGHT,
	VI_FRONT,
	VI_BACK,
	VI_USER
    };

    enum Direction
    {
	DR_FORWARD,
	DR_BACKWARD,
	DR_LEFT,
	DR_RIGHT
    };

    class CameraMan
    {
    public:
		CameraMan(Ogre::Camera* cam)
		: mCamera(0)
		, mTarget(0)
		, mOrbiting(false)
		, mShiftDown(false)
		, mDistFromTarget(0)
		, mCameraNode(0)
		, mCurrentView(VI_USER)
		, mTargetEntity(0)
		, mTopSpeed(15)
		, mVelocity(Ogre::Vector3::ZERO)
		, mGoingForward(false)
		, mGoingBack(false)
		, mGoingLeft(false)
		, mGoingRight(false)
		, mGoingUp(false)
		, mGoingDown(false)
		{
			setCamera(cam);
			setMode(CM_BLENDER);
		}

		virtual ~CameraMan() {}

		/*-----------------------------------------------------------------------------
		| Swaps the camera on our camera man for another camera.
		-----------------------------------------------------------------------------*/
		virtual void setCamera(Ogre::Camera* cam)
		{
			mCamera = cam;
			mCameraNode = mCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			mCameraNode->attachObject( mCamera );
			mCameraNode->pitch( Ogre::Degree( -45.0f ) );
			mCamera->setPosition( Ogre::Vector3::UNIT_Z * 50.0f );
			mCamera->lookAt( Ogre::Vector3( 0.0f, 0.0f, 0.0f ) );
		}

		virtual Ogre::Camera* getCamera()
		{
			return mCamera;
		}

		/*-----------------------------------------------------------------------------
		| Sets the target to revolve around.
		-----------------------------------------------------------------------------*/
		virtual void setTarget(Ogre::SceneNode* target)
		{
		    if (target != mTarget)
		    {
			mTarget = target;
			if(target)
			{
			    setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
			}
			else
			{
			    mCamera->setAutoTracking(false);
			}
		    }
		}

		/*-----------------------------------------------------------------------------
		| Manually stops the camera when in free-look mode.
		-----------------------------------------------------------------------------*/
		virtual void manualStop()
		{
			if (mMode == CM_FLY)
			{
				mGoingForward = false;
				mGoingBack = false;
				mGoingLeft = false;
				mGoingRight = false;
				mGoingUp = false;
				mGoingDown = false;
				mVelocity = Ogre::Vector3::ZERO;
			}
		}

		virtual Ogre::Entity* getTargetEntity()
		{
			return mTargetEntity;
		}

		virtual Ogre::SceneNode* getTarget()
		{
			return mTarget;
		}

		virtual Ogre::Real getDistanceFromTarget()
		{
		    return mDistFromTarget;
		}

		/*-----------------------------------------------------------------------------
		| Sets the spatial offset from the target. Only applies for orbit style.
		-----------------------------------------------------------------------------*/
		virtual void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
		{
			mCamera->setPosition(mTarget->_getDerivedPosition());
			mCamera->setOrientation(mTarget->_getDerivedOrientation());
			mCamera->yaw(yaw);
			mCamera->pitch(-pitch);
			mCamera->moveRelative(Ogre::Vector3(0, 0, dist));
		}

		/*-----------------------------------------------------------------------------
		| Sets the movement style of our camera man.
		-----------------------------------------------------------------------------*/
		virtual void setMode(CameraMode mode)
		{
			if (mMode != CM_BLENDER && mode == CM_BLENDER)
			{
			    setTarget(mTarget ? mTarget : mCamera->getSceneManager()->getRootSceneNode());
			    setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
			}
			else if (mMode != CM_FLY && mode == CM_FLY)
			{
			    mCamera->setAutoTracking(false);
			    mCamera->setFixedYawAxis(true);
			}
			mMode = mode;

		}

		virtual void setProjectionType(Ogre::ProjectionType pt)
		{
		    if (pt == Ogre::PT_ORTHOGRAPHIC)
		    {
			/// @todo: Make orthographic projection work properly.
		    }
		    else if (pt == Ogre::PT_PERSPECTIVE)
		    {
			mCamera->setCustomProjectionMatrix(false);
		    }
		    mCamera->setProjectionType(pt);
		}


		virtual Ogre::ProjectionType getProjectionType()
		{
		    return mCamera->getProjectionType();
		}

		virtual View getView()
		{
		    return mCurrentView;
		}

		virtual void setView(View newView)
		{
		    switch(newView)
		    {
		    case VI_TOP:
			mCameraNode->setOrientation(sqrt(0.5), -sqrt(0.5), 0, 0);
			break;
		    case VI_BOTTOM:
			mCameraNode->setOrientation(sqrt(0.5), sqrt(0.5), 0, 0);
			break;
		    case VI_LEFT:
			mCameraNode->setOrientation(sqrt(0.5), 0, -sqrt(0.5), 0);
			break;
		    case VI_RIGHT:
			mCameraNode->setOrientation(sqrt(0.5), 0, sqrt(0.5), 0);
			break;
		    case VI_FRONT:
			mCameraNode->setOrientation(1, 0, 0, 0);
			break;
		    case VI_BACK:
			setView(VI_FRONT); // Recursion
			mCameraNode->setOrientation(0, 0, 1, 0);

		    }
		    mCurrentView = newView;
		}

		virtual void rotatePerspective(Direction dir)
		{
		    Ogre::Radian amount = Ogre::Radian(Ogre::Degree(15));
		    switch(dir)
		    {
		    case DR_FORWARD:
			mCameraNode->rotate(Ogre::Vector3(1, 0, 0), -amount);
			break;
		    case DR_BACKWARD:
			mCameraNode->rotate(Ogre::Vector3(1, 0, 0), amount);
			break;
		    case DR_LEFT:
			mCameraNode->rotate(Ogre::Vector3(0, 1, 0), -amount, Ogre::Node::TS_WORLD);
			break;
		    case DR_RIGHT:
			mCameraNode->rotate(Ogre::Vector3(0, 1, 0), amount, Ogre::Node::TS_WORLD);
			break;
		    }
		}

		virtual void numpadViewSwitch(const QKeyEvent* evt)
		{
		    bool ctrl = evt->modifiers().testFlag(Qt::ControlModifier);
		    bool numpad = evt->modifiers().testFlag(Qt::KeypadModifier);
		    if (numpad)
		    {
			switch(evt->key())
			{
			case Qt::Key_1:
			    setView(ctrl ? VI_BACK : VI_FRONT);
			    break;
			case Qt::Key_2:
			    rotatePerspective(DR_BACKWARD);
			    break;
			case Qt::Key_3:
			    setView(ctrl ? VI_LEFT : VI_RIGHT);
			    break;
			case Qt::Key_4:
			    rotatePerspective(DR_LEFT);
			    break;
			case Qt::Key_5:
			    setProjectionType((mCamera->getProjectionType() == Ogre::PT_PERSPECTIVE) ? Ogre::PT_ORTHOGRAPHIC : Ogre::PT_PERSPECTIVE);
			    break;
			case Qt::Key_6:
			    rotatePerspective(DR_RIGHT);
			    break;
			case Qt::Key_7:
			    setView(ctrl ? VI_BOTTOM : VI_TOP);
			    break;
			case Qt::Key_8:
			    rotatePerspective(DR_FORWARD);
			    break;
			}
		    }
		}

		virtual CameraMode getMode()
		{
		    return mMode;
		}

		/*-----------------------------------------------------------------------------
		| Per-frame updates.
		-----------------------------------------------------------------------------*/
		virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt)
		{
		    if (mMode == CM_FLY)
		    {
			// build our acceleration vector based on keyboard input composite
			Ogre::Vector3		accel =  Ogre::Vector3::ZERO;
			if (mGoingForward)	accel += mCamera->getDirection();
			if (mGoingBack)		accel -= mCamera->getDirection();
			if (mGoingRight)	accel += mCamera->getRight();
			if (mGoingLeft)		accel -= mCamera->getRight();
			if (mGoingUp)		accel += mCamera->getUp();
			if (mGoingDown)		accel -= mCamera->getUp();

			// if accelerating, try to reach top speed in a certain time
			Ogre::Real topSpeed = mShiftDown ? mTopSpeed * 20 : mTopSpeed;
			if (accel.squaredLength() != 0)
			{
				accel.normalise();
				mVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
			}
			// if not accelerating, try to stop in a certain time
			else mVelocity -= mVelocity * evt.timeSinceLastFrame * 10;

			Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

			// keep camera velocity below top speed and above epsilon
			if (mVelocity.squaredLength() > topSpeed * topSpeed)
			{
				mVelocity.normalise();
				mVelocity *= topSpeed;
			}
			else if (mVelocity.squaredLength() < tooSmall * tooSmall)
				mVelocity = Ogre::Vector3::ZERO;

			if (mVelocity != Ogre::Vector3::ZERO) mCamera->move(mVelocity * evt.timeSinceLastFrame);
		    }
		    return true;
		}

		/*-----------------------------------------------------------------------------
		| Processes key presses for free-look style movement.
		-----------------------------------------------------------------------------*/
		virtual void injectKeyDown(const QKeyEvent *evt)
		{
		    if (mMode == CM_FLY)
		    {
			if(evt->key() == Qt::Key_W)
			    mGoingForward = true;
			else if(evt->key() == Qt::Key_A)
			    mGoingLeft = true;
			else if(evt->key() == Qt::Key_S)
			    mGoingBack = true;
			else if(evt->key() == Qt::Key_D)
			    mGoingRight = true;
		    }
		    if(evt->key() == Qt::Key_Shift)
			mShiftDown = true;
		    if (mMode == CM_BLENDER)
			numpadViewSwitch(evt);
		}

		/*-----------------------------------------------------------------------------
		| Processes key releases for free-look style movement.
		-----------------------------------------------------------------------------*/
		virtual void injectKeyUp(const QKeyEvent* evt)
		{
		    if(evt->key() == Qt::Key_W)
			mGoingForward = false;
		    else if(evt->key() == Qt::Key_A)
			mGoingLeft = false;
		    else if(evt->key() == Qt::Key_S)
			mGoingBack = false;
		    else if(evt->key() == Qt::Key_D)
			mGoingRight = false;
		    if(evt->key() == Qt::Key_Shift)
			mShiftDown = false;
		}

		/*-----------------------------------------------------------------------------
		| Processes mouse movement differently for each style.
		-----------------------------------------------------------------------------*/
		virtual void injectMouseMove(Ogre::Vector2 mousePos)
		{

		    if (mMode == CM_FLY)
		    {
			    mCamera->yaw(Ogre::Degree(-mousePos.x * 0.15f));
			    mCamera->pitch(Ogre::Degree(-mousePos.y * 0.15f));
		    }
		    if (mMode == CM_BLENDER || mMode == CM_ORBIT)
		    {
			    if (mOrbiting && !mShiftDown)
			    {
				rotate(mousePos.x, mousePos.y);
				if (mCurrentView != VI_USER)
				    mCurrentView = VI_USER;
			    }
			    else if ((mOrbiting && mShiftDown) && mMode == CM_BLENDER)
			    {
				pan(mousePos.x, mousePos.y);
			    }
		    }
		}

		virtual void injectMouseWheel(const QWheelEvent* evt)
		{
		    mMouseWheelDelta = evt->delta();

		    mDistFromTarget = (mCamera->getPosition() - mTarget->_getDerivedPosition()).length();
		    mCamera->moveRelative(Ogre::Vector3(0, 0, -mMouseWheelDelta * 0.0008f * mDistFromTarget));
		}

		virtual void injectMouseDown(const QMouseEvent* evt)
		{
		    if (mMode == CM_BLENDER || mMode == CM_ORBIT)
		    {
			if (evt->button() == Qt::MiddleButton)
			{
			    mOrbiting = true;
			}
		    }
		}

		/*-----------------------------------------------------------------------------
		| Processes mouse releases. Only applies for orbit style.
		| Left button is for orbiting, and right button is for zooming.
		-----------------------------------------------------------------------------*/
		virtual void injectMouseUp(const QMouseEvent* evt)
		{
		    if (mMode == CM_BLENDER || mMode == CM_ORBIT)
		    {
			if (evt->button() == Qt::MiddleButton) mOrbiting = false;
		    }
		}

		virtual void rotate(int x, int y)
		{
		    mCameraNode->yaw( Ogre::Degree( -x * 0.4f ), Ogre::Node::TS_PARENT );
		    mCameraNode->pitch( Ogre::Degree( -y * 0.4f ) );
		}

		virtual void pan(int x, int y)
		{
		    Ogre::Vector3 transVector( -x, y, 0 );
		    if (mTarget)
		    {
			mDistFromTarget = (mCamera->getPosition() - mTarget->_getDerivedPosition()).length();
			if (mTarget->getAttachedObject(0))
			    transVector *= mTarget->getAttachedObject(0)->getBoundingRadius() * (mDistFromTarget / 10000.0f);
		    }
		    mCameraNode->translate(transVector, Ogre::Node::TS_LOCAL);
		}

    protected:

		Ogre::Camera* mCamera;

		Ogre::SceneNode* mTarget;
		Ogre::Entity* mTargetEntity;
		Ogre::SceneNode* mCameraNode;

		bool mOrbiting;
		bool mShiftDown;
		bool mGoingForward;
		bool mGoingBack;
		bool mGoingLeft;
		bool mGoingRight;
		bool mGoingUp;
		bool mGoingDown;

		Ogre::Real mDistFromTarget;
		Ogre::Real mTopSpeed;
		Ogre::Vector3 mVelocity;
		int mMouseWheelDelta;

		View mCurrentView;
		CameraMode mMode;


    };
}

#endif
