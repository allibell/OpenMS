// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2018.
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
// $Maintainer: Douglas McCloskey, Pasquale Domenico Colaianni, Svetlana Kutuzova $
// $Authors: Douglas McCloskey, Pasquale Domenico Colaianni, Svetlana Kutuzova $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>
#include <OpenMS/FORMAT/FeatureXMLFile.h>
#include <OpenMS/ANALYSIS/OPENSWATH/MRMBatchFeatureSelector.h>

///////////////////////////
#include <OpenMS/ANALYSIS/OPENSWATH/MRMFeatureSelector.h>
///////////////////////////

#define TRANSITIONTSVREADER_TESTING 1 

using namespace OpenMS;
using namespace std;

START_TEST(MRMFeatureSelector, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

const String features_path = OPENMS_GET_TEST_DATA_PATH("MRMFeatureSelector_150601_0_BloodProject01_PLT_QC_Broth-1_1.featureXML");
const String features_path_small = OPENMS_GET_TEST_DATA_PATH("MRMFeatureSelector_100ug.featureXML");
MRMFeatureSelectorScore* ptr = nullptr;
MRMFeatureSelectorScore* null_ptr = nullptr;

START_SECTION(MRMFeatureSelectorScore())
{
  ptr = new MRMFeatureSelectorScore();
  TEST_NOT_EQUAL(ptr, null_ptr)
}
END_SECTION

START_SECTION(~MRMFeatureSelectorScore())
{
  delete ptr;
}
END_SECTION

START_SECTION(MRMFeatureSelectorScore::selectMRMFeature())
{
  FeatureMap feature_map;
  FeatureXMLFile feature_file;
  feature_file.load(features_path, feature_map);
  TEST_EQUAL(feature_map.size(), 703);

  MRMFeatureSelector::SelectorParameters parameters;

  parameters.select_transition_group = true;
  parameters.segment_window_length = -1;
  parameters.segment_step_length = -1;
  parameters.variable_type = MRMFeatureSelector::VariableType::INTEGER;
  parameters.optimal_threshold = 0.5;
  parameters.score_weights = {
    {"sn_ratio", MRMFeatureSelector::LambdaScore::LOG},
    {"peak_apices_sum", MRMFeatureSelector::LambdaScore::LOG}
  };

  MRMFeatureSelectorScore selectorScore;
  FeatureMap output_selected;
  selectorScore.selectMRMFeature(feature_map, output_selected, parameters);

  TEST_EQUAL(output_selected.size(), 117);

  const Feature& f1 = output_selected[0].getSubordinates()[0];
  TEST_REAL_SIMILAR(f1.getMetaValue("peak_apex_int"), 286.0);
  TEST_STRING_EQUAL(f1.getMetaValue("native_id").toString(), "23dpg.23dpg_1.Heavy");
  TEST_REAL_SIMILAR(f1.getRT(), 16.7592102584839);

  const Feature& f2 = output_selected[50].getSubordinates()[0];
  TEST_REAL_SIMILAR(f2.getMetaValue("peak_apex_int"), 391.5);
  TEST_STRING_EQUAL(f2.getMetaValue("native_id").toString(), "f1p.f1p_1.Heavy");
  TEST_REAL_SIMILAR(f2.getRT(), 8.53021852213542);
}
END_SECTION

START_SECTION(removeSpaces_())
{
  MRMFeatureSelector_test selector;
  TEST_STRING_EQUAL(selector.removeSpaces_("h e ll o"), "hello");
  TEST_STRING_EQUAL(selector.removeSpaces_("hello"), "hello");
  TEST_STRING_EQUAL(selector.removeSpaces_(""), "");
  TEST_STRING_EQUAL(selector.removeSpaces_("A    B"), "AB");
}
END_SECTION

START_SECTION(constructTargTransList_())
{
  MRMFeatureSelector_test selector;
  FeatureMap feature_map;
  FeatureXMLFile feature_file;
  feature_file.load(features_path, feature_map);

  vector<pair<double, String>> time_to_name;
  map<String, vector<Feature>> feature_name_map;

  const bool select_transition_group = true;

  selector.constructTargTransList_(feature_map, time_to_name, feature_name_map, select_transition_group);

  TEST_EQUAL(time_to_name.size(), 117)
  TEST_EQUAL(feature_name_map.size(), 117)

  sort(time_to_name.begin(), time_to_name.end());

  pair<double, String> *p = nullptr;

  p = &time_to_name.front();
  TEST_REAL_SIMILAR(p->first, 0)
  TEST_STRING_EQUAL(p->second, "arg-L")

  p = &time_to_name[1];
  TEST_REAL_SIMILAR(p->first, 0.167913821)
  TEST_STRING_EQUAL(p->second, "orn")

  p = &time_to_name[54];
  TEST_REAL_SIMILAR(p->first, 61.76161499)
  TEST_STRING_EQUAL(p->second, "35cgmp")

  p = &time_to_name[99];
  TEST_REAL_SIMILAR(p->first, 92.88219725)
  TEST_STRING_EQUAL(p->second, "itp")

  p = &time_to_name.back();
  TEST_REAL_SIMILAR(p->first, 99.98770892)
  TEST_STRING_EQUAL(p->second, "succoa")
}
END_SECTION

START_SECTION(weightScore_())
{
  MRMFeatureSelector_test selector;
  double score;

  score = selector.weightScore_(3413.0, MRMFeatureSelector::LambdaScore::LINEAR);
  TEST_REAL_SIMILAR(score, 3413.0)
  score = selector.weightScore_(341.0, MRMFeatureSelector::LambdaScore::INVERSE);
  TEST_REAL_SIMILAR(score, 0.002932551)
  score = selector.weightScore_(341.0, MRMFeatureSelector::LambdaScore::LOG);
  TEST_REAL_SIMILAR(score, 5.831882477)
  score = selector.weightScore_(96640.0, MRMFeatureSelector::LambdaScore::INVERSE_LOG);
  TEST_REAL_SIMILAR(score, 0.087117)
  score = selector.weightScore_(341.0, MRMFeatureSelector::LambdaScore::INVERSE_LOG10);
  TEST_REAL_SIMILAR(score, 0.394827074)
}
END_SECTION

START_SECTION(computeScore_())
{
  MRMFeatureSelector_test selector;
  double score;
  Feature feature;
  feature.setMetaValue("sn_ratio", 6.84619503982874);
  feature.setMetaValue("peak_apices_sum", 96640.0);

  score = selector.computeScore_(feature, {{"sn_ratio", MRMFeatureSelector::LambdaScore::INVERSE_LOG}});
  TEST_REAL_SIMILAR(score, 0.5198334582314795)

  score = selector.computeScore_(feature, {{"peak_apices_sum", MRMFeatureSelector::LambdaScore::INVERSE_LOG10}});
  TEST_REAL_SIMILAR(score, 0.20059549093267626)

  score = selector.computeScore_(feature, {
    {"sn_ratio", MRMFeatureSelector::LambdaScore::INVERSE_LOG},
    {"peak_apices_sum", MRMFeatureSelector::LambdaScore::INVERSE_LOG10}
  });
  TEST_REAL_SIMILAR(score, 0.10427624775717449)
}
END_SECTION

START_SECTION(batchMRMFeaturesQMIP() integer) // integer variable type
{
  FeatureMap feature_map;
  FeatureXMLFile feature_file;
  feature_file.load(features_path_small, feature_map);

  MRMFeatureSelector::SelectorParameters params1;
  params1.nn_threshold = 4;
  params1.locality_weight = "false";
  params1.select_transition_group = "true";
  params1.segment_window_length = 8;
  params1.segment_step_length = 4;
  params1.variable_type = MRMFeatureSelector::VariableType::INTEGER;
  params1.optimal_threshold = 0.5;
  params1.score_weights = {
    {"sn_ratio", MRMFeatureSelector::LambdaScore::INVERSE_LOG},
    {"peak_apices_sum", MRMFeatureSelector::LambdaScore::INVERSE_LOG10}
  };

  MRMFeatureSelector::SelectorParameters params2 = params1;
  params2.segment_window_length = -1;
  params2.segment_step_length = -1;

  FeatureMap output_selected;
  MRMBatchFeatureSelector::batchMRMFeaturesQMIP(feature_map, output_selected, {params1, params2});

  sort(output_selected.begin(), output_selected.end(), [](const Feature& a, const Feature& b){
    return a.getMetaValue("PeptideRef").toString() < b.getMetaValue("PeptideRef").toString(); });

  TEST_EQUAL(output_selected.size(), 8);
  TEST_STRING_EQUAL(output_selected[0].getMetaValue("PeptideRef"), "5-HTP");
  TEST_REAL_SIMILAR(output_selected[0].getRT(), 2.03215546258545);
  TEST_STRING_EQUAL(output_selected[1].getMetaValue("PeptideRef"), "Acetylserotonin");
  TEST_REAL_SIMILAR(output_selected[1].getRT(), 5.07082551965332);
  TEST_STRING_EQUAL(output_selected[2].getMetaValue("PeptideRef"), "Acetyltryptamine");
  TEST_REAL_SIMILAR(output_selected[2].getRT(), 6.96528256036377);
  TEST_STRING_EQUAL(output_selected[3].getMetaValue("PeptideRef"), "Melatonin");
  TEST_REAL_SIMILAR(output_selected[3].getRT(), 6.96528256036377);
  TEST_STRING_EQUAL(output_selected[4].getMetaValue("PeptideRef"), "Riboflavin");
  TEST_REAL_SIMILAR(output_selected[4].getRT(), 5.07082551965332);
  TEST_STRING_EQUAL(output_selected[5].getMetaValue("PeptideRef"), "Serotonin");
  TEST_REAL_SIMILAR(output_selected[5].getRT(), 1.78708603594971);
  TEST_STRING_EQUAL(output_selected[6].getMetaValue("PeptideRef"), "Tryptamine");
  TEST_REAL_SIMILAR(output_selected[6].getRT(), 3.43251273956299);
  TEST_STRING_EQUAL(output_selected[7].getMetaValue("PeptideRef"), "Tryptophan");
  TEST_REAL_SIMILAR(output_selected[7].getRT(), 3.43251273956299);

  // DEBUG
  // sort(output_selected.begin(), output_selected.end(), [](const Feature& a, const Feature& b){ return a.getRT() < b.getRT(); });
  cout << "\n\nSTART DEBUG INFO" << endl;
  for (size_t i = 0; i < output_selected.size(); ++i)
  {
    const Feature& f = output_selected[i];
    cout << "[" << i << "]\t\t" << f.getMetaValue("PeptideRef") << "\t\t" << f.getRT() << endl;
    for (size_t j = 0; j < f.getSubordinates().size(); ++j)
    {
      cout << "[" << i << "][" << j << "]\t\t" << f.getSubordinates()[j].getMetaValue("native_id") << "\t\t" << f.getSubordinates()[j].getMetaValue("peak_apex_int") << endl;
    }
  }
  cout << "END   DEBUG INFO\n" << endl;
}
END_SECTION

START_SECTION(batchMRMFeaturesQMIP() continuous) // continuous variable type
{
  FeatureMap feature_map;
  FeatureXMLFile feature_file;
  feature_file.load(features_path, feature_map);

  MRMFeatureSelector::SelectorParameters params1;
  params1.nn_threshold = 4;
  params1.locality_weight = false;
  params1.select_transition_group = true;
  params1.segment_window_length = 8;
  params1.segment_step_length = 4;
  params1.variable_type = MRMFeatureSelector::VariableType::CONTINUOUS;
  params1.optimal_threshold = 0.5;
  params1.score_weights = {
    {"sn_ratio", MRMFeatureSelector::LambdaScore::INVERSE_LOG},
    {"peak_apices_sum", MRMFeatureSelector::LambdaScore::INVERSE_LOG10}
  };

  MRMFeatureSelector::SelectorParameters params2 = params1;
  params2.segment_window_length = -1;
  params2.segment_step_length = -1;

  FeatureMap output_selected;
  MRMBatchFeatureSelector::batchMRMFeaturesQMIP(feature_map, output_selected, {params1, params2});

  sort(output_selected.begin(), output_selected.end(), [](const Feature& a, const Feature& b){
    return a.getMetaValue("PeptideRef").toString() < b.getMetaValue("PeptideRef").toString(); });

  TEST_EQUAL(output_selected.size(), 82);

  const Feature* f = &output_selected[0].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 262623.5);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "23dpg.23dpg_1.Heavy");
  TEST_REAL_SIMILAR(f->getRT(), 15.8944563381195);

  f = &output_selected[1].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 3678.5);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "2obut.2obut_1.Heavy");
  TEST_REAL_SIMILAR(f->getRT(), 9.54183669713338);

  f = &output_selected[2].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 2332.0);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "5oxpro.5oxpro_1.Heavy");
  TEST_REAL_SIMILAR(f->getRT(), 3.67159563852946);

  f = &output_selected[50].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 37090.0);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "gua.gua_1.Heavy");
  TEST_REAL_SIMILAR(f->getRT(), 1.27875684076945);

  // // DEBUG
  // // sort(output_selected.begin(), output_selected.end(), [](const Feature& a, const Feature& b){ return a.getRT() < b.getRT(); });
  // for (const Feature& f : output_selected)
  // {
  //   cout << f.getMetaValue("PeptideRef") << "\t" << f << endl;
  // }
}
END_SECTION

START_SECTION(batchMRMFeaturesQMIP() continuous but smaller experiment) // continuous variable type
{
  FeatureMap feature_map;
  FeatureXMLFile feature_file;
  feature_file.load(features_path_small, feature_map);

  MRMFeatureSelector::SelectorParameters params1;
  params1.nn_threshold = 4;
  params1.locality_weight = false;
  params1.select_transition_group = true;
  params1.segment_window_length = 8;
  params1.segment_step_length = 4;
  params1.variable_type = MRMFeatureSelector::VariableType::CONTINUOUS;
  params1.optimal_threshold = 0.5;
  params1.score_weights = {
    {"sn_ratio", MRMFeatureSelector::LambdaScore::INVERSE_LOG},
    {"peak_apices_sum", MRMFeatureSelector::LambdaScore::INVERSE_LOG10}
  };

  MRMFeatureSelector::SelectorParameters params2 = params1;
  params2.segment_window_length = -1;
  params2.segment_step_length = -1;

  FeatureMap output_selected;
  MRMBatchFeatureSelector::batchMRMFeaturesQMIP(feature_map, output_selected, {params1, params2});

  sort(output_selected.begin(), output_selected.end(), [](const Feature& a, const Feature& b){
    return a.getMetaValue("PeptideRef").toString() < b.getMetaValue("PeptideRef").toString(); });

  TEST_EQUAL(output_selected.size(), 8);

  const Feature* f = &output_selected[0].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 29.5228353632885);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "5-HTP");
  TEST_REAL_SIMILAR(f->getRT(), 2.03215546258545);

  f = &output_selected[1].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 30.7684884945637);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Acetylserotonin");
  TEST_REAL_SIMILAR(f->getRT(), 5.07082551965332);

  f = &output_selected[2].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 28.4325753928028);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Acetyltryptamine");
  TEST_REAL_SIMILAR(f->getRT(), 6.96528256036377);

  f = &output_selected[3].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 28.4325753928028);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Melatonin");
  TEST_REAL_SIMILAR(f->getRT(), 6.96528256036377);

  f = &output_selected[4].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 30.7684884945637);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Riboflavin");
  TEST_REAL_SIMILAR(f->getRT(), 5.07082551965332);

  f = &output_selected[5].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 22.6054459245013);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Serotonin");
  TEST_REAL_SIMILAR(f->getRT(), 1.78708603594971);

  f = &output_selected[6].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 37.9693079695627);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Tryptamine");
  TEST_REAL_SIMILAR(f->getRT(), 3.43251273956299);

  f = &output_selected[7].getSubordinates()[0];
  TEST_REAL_SIMILAR(f->getMetaValue("peak_apex_int"), 37.9693079695627);
  TEST_STRING_EQUAL(f->getMetaValue("native_id"), "Tryptophan");
  TEST_REAL_SIMILAR(f->getRT(), 3.43251273956299);

  // // DEBUG
  // // sort(output_selected.begin(), output_selected.end(), [](const Feature& a, const Feature& b){ return a.getRT() < b.getRT(); });
  cout << "\n\nSTART DEBUG INFO" << endl;
  for (size_t i = 0; i < output_selected.size(); ++i)
  {
    const Feature& f = output_selected[i];
    cout << "[" << i << "]\t\t" << f.getMetaValue("PeptideRef") << "\t\t" << f.getRT() << endl;
    for (size_t j = 0; j < f.getSubordinates().size(); ++j)
    {
      cout << "[" << i << "][" << j << "]\t\t" << f.getSubordinates()[j].getMetaValue("native_id") << "\t\t" << f.getSubordinates()[j].getMetaValue("peak_apex_int") << endl;
    }
  }
  cout << "END   DEBUG INFO\n" << endl;
}
END_SECTION

END_TEST
