AM_CPPFLAGS = -I ../include \
              -I ../hts_engine_API/include \
              -I converter \
              -I hts_engine_API \
              -I japanese \
              -I label \
              -I score \
              -I temporary \
              -I util \
              -I xml

lib_LIBRARIES = libSinsy.a

libSinsy_a_SOURCES = Sinsy.cpp \
                     ./converter/ConfGroup.cpp \
                     ./converter/ConfGroup.h \
                     ./converter/ConfManager.cpp \
                     ./converter/ConfManager.h \
                     ./converter/Converter.cpp \
                     ./converter/Converter.h \
                     ./converter/GeneralConf.cpp \
                     ./converter/GeneralConf.h \
                     ./converter/IConf.h \
                     ./converter/IConvertable.h \
                     ./converter/PhonemeInfo.cpp \
                     ./converter/PhonemeInfo.h \
                     ./converter/UnknownConf.cpp \
                     ./converter/UnknownConf.h \
                     ./converter/util_converter.cpp \
                     ./converter/util_converter.h \
                     ./hts_engine_API/HtsEngine.cpp \
                     ./hts_engine_API/HtsEngine.h \
                     ./hts_engine_API/SynthConditionImpl.cpp \
                     ./hts_engine_API/SynthConditionImpl.h \
                     ./japanese/JConf.cpp \
                     ./japanese/JConf.h \
                     ./label/ILabelOutput.h \
                     ./label/INoteLabel.h \
                     ./label/IPhonemeLabel.h \
                     ./label/ISyllableLabel.h \
                     ./label/LabelData.cpp \
                     ./label/LabelData.h \
                     ./label/LabelMaker.cpp \
                     ./label/LabelMaker.h \
                     ./label/LabelMeasure.cpp \
                     ./label/LabelMeasure.h \
                     ./label/LabelPosition.cpp \
                     ./label/LabelPosition.h \
                     ./label/LabelStream.cpp \
                     ./label/LabelStream.h \
                     ./label/LabelStrings.cpp \
                     ./label/LabelStrings.h \
                     ./label/NoteGroup.cpp \
                     ./label/NoteGroup.h \
                     ./label/NoteLabeler.cpp \
                     ./label/NoteLabeler.h \
                     ./label/PhonemeLabeler.cpp \
                     ./label/PhonemeLabeler.h \
                     ./label/SyllableLabeler.cpp \
                     ./label/SyllableLabeler.h \
                     ./score/Beat.cpp \
                     ./score/Beat.h \
                     ./score/Dynamics.cpp \
                     ./score/Dynamics.h \
                     ./score/IScoreWritable.h \
                     ./score/IScoreWriter.h \
                     ./score/Key.cpp \
                     ./score/Key.h \
                     ./score/Mode.cpp \
                     ./score/Mode.h \
                     ./score/Note.cpp \
                     ./score/Note.h \
                     ./score/Pitch.cpp \
                     ./score/Pitch.h \
                     ./score/ScorePosition.cpp \
                     ./score/ScorePosition.h \
                     ./score/Slur.cpp \
                     ./score/Slur.h \
                     ./score/Syllabic.cpp \
                     ./score/Syllabic.h \
                     ./score/util_score.cpp \
                     ./score/util_score.h \
                     ./temporary/ScoreDoctor.cpp \
                     ./temporary/ScoreDoctor.h \
                     ./temporary/TempScore.cpp \
                     ./temporary/TempScore.h \
                     ./util/Configurations.cpp \
                     ./util/Configurations.h \
                     ./util/Deleter.h \
                     ./util/DiphthongConverter.cpp \
                     ./util/DiphthongConverter.h \
                     ./util/ForEachAdapter.h \
                     ./util/IReadableStream.h \
                     ./util/IStringable.cpp \
                     ./util/IStringable.h \
                     ./util/IWritableStream.h \
                     ./util/InputFile.cpp \
                     ./util/InputFile.h \
                     ./util/MacronTable.cpp \
                     ./util/MacronTable.h \
                     ./util/MultibyteCharRange.cpp \
                     ./util/MultibyteCharRange.h \
                     ./util/OutputFile.cpp \
                     ./util/OutputFile.h \
                     ./util/PhonemeTable.cpp \
                     ./util/PhonemeTable.h \
                     ./util/StreamException.h \
                     ./util/StringTokenizer.cpp \
                     ./util/StringTokenizer.h \
                     ./util/WritableStrStream.h \
                     ./util/util_log.h \
                     ./util/util_string.cpp \
                     ./util/util_string.h \
                     ./util/util_types.h \
                     ./xml/XmlData.cpp \
                     ./xml/XmlData.h \
                     ./xml/XmlParser.cpp \
                     ./xml/XmlParser.h \
                     ./xml/XmlReader.cpp \
                     ./xml/XmlReader.h \
                     ./xml/XmlWriter.cpp \
                     ./xml/XmlWriter.h \
                     ./xml/xml_tags.h \
                     ../hts_engine_API/lib/HTS_audio.c \
                     ../hts_engine_API/lib/HTS_engine.c \
                     ../hts_engine_API/lib/HTS_gstream.c \
                     ../hts_engine_API/lib/HTS_label.c \
                     ../hts_engine_API/lib/HTS_model.c \
                     ../hts_engine_API/lib/HTS_pstream.c \
                     ../hts_engine_API/lib/HTS_sstream.c \
                     ../hts_engine_API/lib/HTS_vocoder.c \
                     ../hts_engine_API/lib/HTS_misc.c

DISTCLEANFILES = *.log *.out *~ */*~

MAINTAINERCLEANFILES = Makefile.in
