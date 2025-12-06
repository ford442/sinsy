/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2015  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the Sinsy working group nor the names of    */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include "GeneralConf.h"
#include "util_log.h"
#include "util_converter.h"
#include "StringTokenizer.h"
#include <map>
#include <fstream>

namespace sinsy
{

class GeneralConf::Impl
{
public:
    typedef std::map<std::string, std::vector<PhonemeInfo> > PhonemeMap;
    PhonemeMap phonemeMap;

    bool convert(const std::string& lyric, IConvertable& convertable) const {
        PhonemeMap::const_iterator itr = phonemeMap.find(lyric);
        if (itr == phonemeMap.end()) {
            return false;
        }
        convertable.addInfo(itr->second, "", "");
        return true;
    }
};

GeneralConf::GeneralConf(const std::string& enc) : IConf(enc), impl(new Impl)
{
}

GeneralConf::~GeneralConf()
{
    delete impl;
}

bool GeneralConf::read(const std::string& table, const std::string& conf, const std::string& macron)
{
    std::ifstream ifs(table.c_str());
    if (!ifs) {
        ERR_MSG("Cannot open " << table);
        return false;
    }

    std::string line;
    while (getline(ifs, line)) {
        StringTokenizer tokenizer(line, " ");
        if (tokenizer.size() < 2) continue;

        std::string word = tokenizer.at(0);
        std::vector<PhonemeInfo> phonemes;
        for (size_t i = 1; i < tokenizer.size(); ++i) {
            phonemes.push_back(PhonemeInfo(tokenizer.at(i)));
        }
        impl->phonemeMap[word] = phonemes;
    }
    return true;
}

bool GeneralConf::convert(const std::string& lyric, IConvertable& convertable) const
{
    return impl->convert(lyric, convertable);
}

};
