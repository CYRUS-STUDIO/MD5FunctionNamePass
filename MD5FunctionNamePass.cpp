#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/MD5.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

struct MD5FunctionNamePass : public PassInfoMixin<MD5FunctionNamePass> {

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

        // 获取函数名
        std::string originalName = F.getName().str();

        // 跳过外部声明（非定义的函数）
        if (F.isDeclaration()) {
            errs() << "Skipping external function: " << originalName << "\n";
            return PreservedAnalyses::all(); // IR 未被修改
        }

        // 排除标准库函数名
        std::set<std::string> standardFunctions = {"printf", "sprintf", "vsprintf"};
        if (standardFunctions.count(F.getName().str())) {
            errs() << "Skipping standard library function: " << F.getName() << "\n";
            return PreservedAnalyses::all();
        }

        // 排除 comdat 函数
        if (F.getComdat()) {
            errs() << "Skipping comdat function: " << F.getName() << "\n";
            return PreservedAnalyses::all();
        }

        // 排除 "main" 函数
        if (originalName == "main") {
            errs() << "Skipping encryption for function: " << originalName << "\n";
            return PreservedAnalyses::all();
        }

        // 打印原始函数名
        errs() << "Original Function Name: " << originalName << "\n";

        // 计算 MD5 哈希值
        MD5 hash;
        MD5::MD5Result result;
        hash.update(originalName);
        hash.final(result);

        // 转换 MD5 结果为字符串
        SmallString<32> hashString;
        MD5::stringifyResult(result, hashString);

        errs() << "MD5 Hash: " << hashString << "\n";

        // 修改函数名为哈希值
        F.setName(hashString);

        return PreservedAnalyses::none(); // 函数 IR 已被修改
    }


    // 当函数带有 optnone 属性时，Pass 默认会被跳过。通过重写 isRequired，可以强制框架认为当前 Pass 对所有函数都是必要的。
    static bool isRequired() {
        return true;
    }
};

// 插件入口
llvm::PassPluginLibraryInfo getPassPluginInfo() {

    errs() << "MD5FunctionNamePass Plugin Loaded Successfully.\n";

    return {LLVM_PLUGIN_API_VERSION, "MD5FunctionNamePass", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](StringRef Name, FunctionPassManager &FPM,
                           ArrayRef<PassBuilder::PipelineElement>) {
                            if (Name == "md5-function-name-pass") {
                                FPM.addPass(MD5FunctionNamePass());
                                return true;
                            }
                            return false;
                        });
            }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getPassPluginInfo();
}
