// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry               
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
// 
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution 
//    may be used to endorse or promote products derived from this software 
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS. 
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING 
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg$
// $Authors: Timo Sachsenberg$
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>

///////////////////////////
#include <OpenMS/ANALYSIS/RNPXL/PScore.h>
///////////////////////////

using namespace OpenMS;
using namespace std;

START_TEST(PScore, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PScore* ptr = 0;
PScore* null_ptr = 0;
START_SECTION(PScore())
{
	ptr = new PScore();
	TEST_NOT_EQUAL(ptr, null_ptr)
}
END_SECTION

START_SECTION(~PScore())
{
	delete ptr;
}
END_SECTION

START_SECTION((static std::vector<Size> calculateIntensityRankInMZWindow(const std::vector< double > &mz, const std::vector< double > &intensities, double mz_window)))
{
  // TODO
}
END_SECTION

START_SECTION((static std::vector<std::vector<Size> > calculateRankMap(const PeakMap &peak_map, double mz_window=100)))
{
  // TODO
}
END_SECTION

START_SECTION((static std::map<Size, PeakSpectrum > calculatePeakLevelSpectra(const PeakSpectrum &spec, const std::vector< Size > &ranks, Size min_level=2, Size max_level=10)))
{
  // TODO
}
END_SECTION

START_SECTION((static double computePScore(double fragment_mass_tolerance, bool fragment_mass_tolerance_unit_ppm, const std::map< Size, PeakSpectrum > &peak_level_spectra, const std::vector< RichPeakSpectrum > &theo_spectra, double mz_window=100.0)))
{
  // TODO
}
END_SECTION

START_SECTION((static double computePScore(double fragment_mass_tolerance, bool fragment_mass_tolerance_unit_ppm, const std::map< Size, PeakSpectrum > &peak_level_spectra, const RichPeakSpectrum &theo_spectrum, double mz_window=100.0)))
{
  // TODO
}
END_SECTION

START_SECTION((static double massCorrectionTerm(double mass)))
{
  // TODO
}
END_SECTION

START_SECTION((static double cleavageCorrectionTerm(Size cleavages, bool consecutive_cleavage)))
{
  // TODO
}
END_SECTION

START_SECTION((static double modificationCorrectionTerm(Size modifications)))
{
  // TODO
}
END_SECTION


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST



