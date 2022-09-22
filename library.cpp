//=============================================================================
// FILE:
//    HelloWorld.cpp
//
// DESCRIPTION:
//    遍历所有在模块中的函数，打印它们的名字和参数通过stderr.严格来讲，这是一个分析pass，
//    如这个函数并没有被修改. 所有的分析Pass必须实现print方法，此次使用err
//
// USAGE:
//    1. 层次化插件管理器
//      opt -load libHelloWorld.dylib -legacy-hello-world -disable-output `\`
//        <input-llvm-file>
//    2. 新插件管理器
//      opt -load-pass-plugin=libHelloWorld.dylib -passes="hello-world" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================
#include "library.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// HelloWorld implementation
//-----------------------------------------------------------------------------
// 不需要向外部暴露借口，尽在匿名类内部实现
namespace {

// 这个方法实现这个Pass做了什么
    void visitor(Function &F) {
        errs() << "(Kevin-Petri) Function from: "<< F.getName() << "\n";
        errs() << "(Kevin-Petri)   number of arguments: " << F.arg_size() << "\n";
    }

// class Kevin : PassInfoMixin<Hll

    // 新插件管理器实现
    struct HelloWorld : PassInfoMixin<HelloWorld> {
        // 主entry点, 将IR单元代入Pass on (&F) and 相应的Pass管理器(询问是否需要)
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
            visitor(F);
            return PreservedAnalyses::all();
        }

        // 如果 isRequired 返回 true,则使用 optnone LLVM 属性装饰的函数将跳过此Pass.
        // 请注意,clang -O0 使用 optnone 修饰所有函数。
        static bool isRequired() { return true; }
    };

// 旧版Pass管理器
    struct LegacyHelloWorld : public FunctionPass {
        static char ID;
        LegacyHelloWorld() : FunctionPass(ID) {}
        // 主入口点 - name传达了要在哪个 IR 单元上运行。
        bool runOnFunction(Function &F) override {
            visitor(F);
            // 不修改 IR 的input unit，因此为`false`
            return false;
        }
    };
}

//-----------------------------------------------------------------------------
// 新PM注册器
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](StringRef Name, FunctionPassManager &FPM,
                           ArrayRef<PassBuilder::PipelineElement>) {
                            if (Name == "hello-world") {
                                FPM.addPass(HelloWorld());
                                return true;
                            }
                            return false;
                        });
            }};
}

// 这是 pass 插件的核心接口. 它保证'opt'在添加到命令行上的传递管道时
// 能够识别HelloWorld, 即通过'-passes = hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getHelloWorldPluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
// 该变量的地址用于唯一标识通行证. 实际值无关紧要。
char LegacyHelloWorld::ID = 0;

// 这是 pass 插件的核心接口. 它保证'opt'在命令行上添加到传递管道时
// 将识别LegacyHelloWorld, 即通过'--legacy-hello-world'
static RegisterPass<LegacyHelloWorld>
        X("legacy-hello-world", "Hello World Pass",
          true, // This pass doesn't modify the CFG => true
          false // This pass is not a pure analysis pass => false
);
