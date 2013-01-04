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

#include <mrpt/vision.h>  // Precompiled headers

#include <mrpt/vision/CVideoFileWriter.h>

// Universal include for all versions of OpenCV
#include <mrpt/otherlibs/do_opencv_includes.h> 

#define M_WRITER (const_cast<CvVideoWriter*>( static_cast<const CvVideoWriter*>(m_video.get())) )
#define M_WRITER_PTR (reinterpret_cast<CvVideoWriter**>(m_video.getPtrToPtr()))


using namespace mrpt;
using namespace mrpt::vision;
using namespace mrpt::system;
using namespace mrpt::utils;

/* ----------------------------------------------------------
						Ctor
   ---------------------------------------------------------- */
CVideoFileWriter::CVideoFileWriter() : m_video(), m_img_size(0,0)
{
}

/* ----------------------------------------------------------
						Dtor
   ---------------------------------------------------------- */
CVideoFileWriter::~CVideoFileWriter()
{
	close();
}

/* ----------------------------------------------------------
						open
   ---------------------------------------------------------- */
bool CVideoFileWriter::open(
	const std::string &out_file,
	double fps,
	const mrpt::vision::TImageSize & frameSize,
	const std::string &fourcc,
	bool isColor )
{
#if MRPT_HAS_OPENCV
	close();

	int cc;

	if (fourcc.empty())
	{
#if MRPT_OPENCV_VERSION_NUM<=0x100
		cc = 0; // Default
#else
	#ifdef MRPT_OS_WINDOWS
		cc = CV_FOURCC_DEFAULT; // Default CV_FOURCC_PROMPT;
	#else
		cc = CV_FOURCC_DEFAULT; // Default
	#endif
#endif
	}
	else
	if (fourcc.size()==4)
	{
		cc = CV_FOURCC( fourcc[0],fourcc[1],fourcc[2],fourcc[3] );
	}
	else
	{
		std::cerr << "[CVideoFileWriter::open] fourcc string must be four character length or empty for default." << std::endl;
		return false;
	}

	m_video = cvCreateVideoWriter(out_file.c_str(),cc,fps,cvSize(frameSize.x,frameSize.y),isColor ? 1:0);

	m_img_size = frameSize;

	return m_video.get() != NULL;
#else
	std::cerr << "[CVideoFileWriter::open] ERROR: MRPT was compiled without OpenCV support "  << std::endl;
	return false;
#endif
}


/* ----------------------------------------------------------
						close
   ---------------------------------------------------------- */
void CVideoFileWriter::close()
{
#if MRPT_HAS_OPENCV
	if (!M_WRITER) return;
	cvReleaseVideoWriter( M_WRITER_PTR );
	*M_WRITER_PTR = NULL;
#endif
}

/* ----------------------------------------------------------
						isOpen
   ---------------------------------------------------------- */
bool CVideoFileWriter::isOpen() const
{
#if MRPT_HAS_OPENCV
	return (M_WRITER!=NULL);
#else
	return false;
#endif
}


/* ----------------------------------------------------------
						operator <<
   ---------------------------------------------------------- */
const CVideoFileWriter& CVideoFileWriter::operator << (const mrpt::utils::CImage& img) const
{
	if (!m_video.get())
		THROW_EXCEPTION("Call open first")

	if ((size_t)m_img_size.x!=img.getWidth() || (size_t)m_img_size.y!=img.getHeight())
		THROW_EXCEPTION(format("Video frame size is %ix%i but image is %ux%u", m_img_size.x,m_img_size.y,(unsigned)img.getWidth(),(unsigned)img.getHeight() ));

#if MRPT_HAS_OPENCV
	if (! cvWriteFrame( M_WRITER, img.getAs<IplImage>() ) )
		THROW_EXCEPTION("Error writing image frame to video file")
#endif
	return *this;
}

/* ----------------------------------------------------------
						writeImage
   ---------------------------------------------------------- */
bool CVideoFileWriter::writeImage(const mrpt::utils::CImage& img) const
{
	if (!m_video.get())
		return false;

	if ((size_t)m_img_size.x!=img.getWidth() || (size_t)m_img_size.y!=img.getHeight())
	{
		std::cout << format("[CVideoFileWriter::writeImage] Error: video frame size is %ix%i but image is %ux%u", m_img_size.x,m_img_size.y,(unsigned)img.getWidth(),(unsigned)img.getHeight() ) << std::endl;
		return false;
	}

#if MRPT_HAS_OPENCV
	return 0!= cvWriteFrame( M_WRITER, img.getAs<IplImage>() );
#else
	return false;
#endif
}

