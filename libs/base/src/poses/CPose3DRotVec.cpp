/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2013, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2013, MAPIR group, University of Malaga                |
   | Copyright (c) 2012-2013, University of Almeria                            |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */

#include <mrpt/base.h>  // Precompiled headers

#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/poses/CPose3DRotVec.h>
#include <mrpt/poses/CPoint3D.h>
#include <mrpt/poses/CPose3D.h>

#include <mrpt/math/utils.h>
#include <mrpt/math/geometry.h>
#include <mrpt/math/CMatrix.h>

using namespace mrpt;
using namespace mrpt::math;
using namespace mrpt::utils;
using namespace mrpt::poses;

IMPLEMENTS_SERIALIZABLE(CPose3DRotVec, CSerializable ,mrpt::poses)

MRPT_TODO("Complete missing methods")

/*---------------------------------------------------------------
	Constructors
  ---------------------------------------------------------------*/
CPose3DRotVec::CPose3DRotVec(const math::CMatrixDouble44 &m)
{
    THROW_EXCEPTION("TODO")
}

CPose3DRotVec::CPose3DRotVec(const CPose3D &m)
{
    THROW_EXCEPTION("TODO")
}

/** Constructor from a quaternion (which only represents the 3D rotation part) and a 3D displacement. */
CPose3DRotVec::CPose3DRotVec(const mrpt::math::CQuaternionDouble &q, const double _x, const double _y, const double _z )
{
    m_coords[0]=_x; m_coords[1]=_y; m_coords[2]=_z;
    THROW_EXCEPTION("TODO")
}

/*---------------------------------------------------------------
   Implements the writing to a CStream capability of
     CSerializable objects
  ---------------------------------------------------------------*/
void  CPose3DRotVec::writeToStream(CStream &out,int *version) const
{
	if (version)
		*version = 0;
	else
	{
		out << m_coords[0] << m_coords[1] << m_coords[2] << m_rotvec[0] << m_rotvec[1] << m_rotvec[2];
	}
}

/*---------------------------------------------------------------
	Implements the reading from a CStream capability of
		CSerializable objects
  ---------------------------------------------------------------*/
void  CPose3DRotVec::readFromStream(CStream &in,int version)
{
	switch(version)
	{
	case 0:
		{
            in >> m_coords[0] >> m_coords[1] >> m_coords[2] >> m_rotvec[0] >> m_rotvec[1] >> m_rotvec[2];
		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)
	};
}

/**  Textual output stream function.
 */
std::ostream& mrpt::poses::operator << (std::ostream& o, const CPose3DRotVec& p)
{
	const std::streamsize old_pre = o.precision();
	const std::ios_base::fmtflags old_flags = o.flags();
	o << "(x,y,z,vx,vy,vz)=(" << std::fixed << std::setprecision(4) << p.m_coords[0] << "," << p.m_coords[1] << "," << p.m_coords[2] <<  ","
	  << p.m_rotvec[0] << "," << p.m_rotvec[1] << "," << p.m_rotvec[2];
	o.flags(old_flags);
	o.precision(old_pre);
	return o;
}

/** Get the 3x3 rotation matrix \sa getHomogeneousMatrix  */
void CPose3DRotVec::getRotationMatrix( mrpt::math::CMatrixDouble33 & ROT ) const
{
    THROW_EXCEPTION("TODO")
}

/*---------------------------------------------------------------
		sphericalCoordinates
---------------------------------------------------------------*/
void CPose3DRotVec::sphericalCoordinates(
    const TPoint3D &point,
    double &out_range,
    double &out_yaw,
    double &out_pitch ) const
{
    // Pass to coordinates as seen from this 6D pose:
	TPoint3D local;
	this->inverseComposePoint(point.x,point.y,point.z, local.x,local.y,local.z);

    // Range:
	out_range = local.norm();

    // Yaw:
    if (local.y!=0 || local.x!=0)
         out_yaw = atan2(local.y,local.x);
    else out_yaw = 0;

    // Pitch:
    if (out_range!=0)
         out_pitch = -asin( local.z / out_range );
    else out_pitch = 0;
}

/*---------------------------------------------------------------
		composePoint
---------------------------------------------------------------*/
void CPose3DRotVec::composePoint(double lx,double ly,double lz, double &gx, double &gy, double &gz,
	mrpt::math::CMatrixFixedNumeric<double,3,3>  *out_jacobian_df_dpoint,
	mrpt::math::CMatrixFixedNumeric<double,3,6>  *out_jacobian_df_dpose) const
{
    THROW_EXCEPTION("TODO")
}

/*---------------------------------------------------------------
		unary -
---------------------------------------------------------------*/
CPose3DRotVec mrpt::poses::operator -(const CPose3DRotVec &b)
{
    THROW_EXCEPTION("TODO")
}

bool mrpt::poses::operator==(const CPose3DRotVec &p1,const CPose3DRotVec &p2)
{
	return (p1.m_coords==p2.m_coords)&&(p1.m_rotvec==p2.m_rotvec);
}

bool mrpt::poses::operator!=(const CPose3DRotVec &p1,const CPose3DRotVec &p2)
{
	return (p1.m_coords!=p2.m_coords)||(p1.m_rotvec!=p2.m_rotvec);
}

/*---------------------------------------------------------------
				point3D = pose3D + point3D
  ---------------------------------------------------------------*/
CPoint3D  CPose3DRotVec::operator + (const CPoint3D& b) const
{
    THROW_EXCEPTION("TODO")
}

/*---------------------------------------------------------------
				point3D = pose3D + point2D
  ---------------------------------------------------------------*/
CPoint3D  CPose3DRotVec::operator + (const CPoint2D& b) const
{
    THROW_EXCEPTION("TODO")
}


/*---------------------------------------------------------------
				this = A + B
  ---------------------------------------------------------------*/
void CPose3DRotVec::composeFrom(const CPose3DRotVec& A, const CPose3DRotVec& B )
{
    THROW_EXCEPTION("TODO")
}

/** Convert this pose into its inverse, saving the result in itself. */
void CPose3DRotVec::inverse()
{
    THROW_EXCEPTION("TODO")
}

/**  Makes \f$ this = A \ominus B \f$ this method is slightly more efficient than "this= A - B;" since it avoids the temporary object.
  *  \note A or B can be "this" without problems.
  * \sa composeFrom, composePoint
  */
void CPose3DRotVec::inverseComposeFrom(const CPose3DRotVec& A, const CPose3DRotVec& B )
{
	// this    =    A  (-)  B
	// HM_this = inv(HM_B) * HM_A
	//
	// [  R_b  | t_b ] -1   [  R_a  | t_a ]    [ R_b^t * Ra |    ..    ]
	// [ ------+-----]    * [ ------+-----]  = [ ---------- +----------]
	// [ 0 0 0 |  1  ]      [ 0 0 0 |  1  ]    [  0  0   0  |      1   ]
	//
    THROW_EXCEPTION("TODO")
}

/**  Computes the 3D point L such as \f$ L = G \ominus this \f$.
  * \sa composePoint, composeFrom
  */
void CPose3DRotVec::inverseComposePoint(const double gx,const double gy,const double gz,double &lx,double &ly,double &lz,
	mrpt::math::CMatrixFixedNumeric<double,3,3>  *out_jacobian_df_dpoint,
	mrpt::math::CMatrixFixedNumeric<double,3,6>  *out_jacobian_df_dpose )const
{
    THROW_EXCEPTION("TODO")
}


/** Exponentiate a Vector in the SE3 Lie Algebra to generate a new CPose3DRotVec.
  */
CPose3DRotVec CPose3DRotVec::exp(const mrpt::math::CArrayDouble<6> & mu)
{
    return CPose3DRotVec(mu);
}

/** Take the logarithm of the 3x3 rotation matrix, generating the corresponding vector in the Lie Algebra.
  */
CArrayDouble<3> CPose3DRotVec::ln_rotation() const
{
	return m_rotvec;
}


/** Take the logarithm of the 3x4 matrix defined by this pose, generating the corresponding vector in the SE3 Lie Algebra.
  */
void CPose3DRotVec::ln(CArrayDouble<6> &result) const
{
    result.head<3>() = m_coords;
    result.tail<3>() = m_rotvec;
}
