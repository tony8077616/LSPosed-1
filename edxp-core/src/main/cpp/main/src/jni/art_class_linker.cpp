
#include <jni.h>
#include <native_util.h>
#include <art/runtime/class_linker.h>
#include <nativehelper/jni_macros.h>
#include <vector>
#include "art_class_linker.h"

namespace edxp {

    static std::vector<void *> deopted_methods;

    static void ClassLinker_setEntryPointsToInterpreter(JNI_START, jobject method) {
        void *reflected_method = env->FromReflectedMethod(method);
        if (std::find(deopted_methods.begin(), deopted_methods.end(), reflected_method) !=
            deopted_methods.end()) {
            LOGD("method %p has been deopted before, skip...", reflected_method);
            return;
        }
        LOGD("deoptimizing method: %p", reflected_method);
        art::ClassLinker::Current()->SetEntryPointsToInterpreter(reflected_method);
        deopted_methods.push_back(reflected_method);
        LOGD("method deoptimized: %p", reflected_method);
    }

    static JNINativeMethod gMethods[] = {
            NATIVE_METHOD(ClassLinker, setEntryPointsToInterpreter, "(Ljava/lang/reflect/Member;)V")
    };

    void RegisterArtClassLinker(JNIEnv *env) {
        REGISTER_EDXP_NATIVE_METHODS(kClassLinkerClassName);
    }

}