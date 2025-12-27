void compileCode(std::string code) { //ts kinda tuff ngl
    const std::vector<std::string> functions = {
        "echo",
        "test",
        "drawText"
    };

#if debugInfo
    std::cout << "compiling code: " << code << "\n";
#endif

    for (int i = 0; i < 1; i++) {
        struct Command {
            int opcode = 0;
            std::vector<std::string> args;
        } command;
        std::vector<struct Command> bytecode;




        std::string output;
        std::string fn;
        std::string arg;
        bool argMode = false;
        bool insideString = false;
        bool error = false;
        int dataType = -1; //0 = var, 1 = integer, 2 = bool, 3 = string
        int argN = 0;
        int k = 0;
        int j = 0;


        for (int i = 0; i < code.length(); i++) {
            char token = code[i];
            switch (token) {
            case '(':
                if (argMode) {
                    if (dataType == -1) {
                        if (isdigit(token)) {
                            dataType = 1;
                            continue;
                        }
                        else if (
                            (code.substr(i, i + 5) == "false") ||
                            (code.substr(i, i + 4) == "true")
                            ) {
                            dataType = 2;
                            std::cout << "FACKING WHAAAAAAAAAAAT";
                            continue;
                        }
                        else if (token == '\"') {
                            dataType = 3;
                            continue;
                        }
                        else if (code[i + 1] != '\"') {
                            dataType = 0;
                            continue;
                        }
                        else {
                            fn = output;
                            output = "";
                            argMode = true;
                            continue;
                        }
                    }
                    else {
                        output += token;
                    }
            case ')':
                arg = output;
            default:
                if ((token != ' ' && !insideString) || insideString) {
                    output += token;
                }
                }
            }

            argMode = false;
            insideString = false;

            int fnIndex = std::find(functions.begin(), functions.end(), fn) - functions.begin();

#if debugInfo

            std::cout << "output:" << output << "\n";
            std::cout << "function: " << fn << "\n";


            std::cout << "function id: " << fnIndex << "\n";

#endif

            command.args.resize(j + 1);

            command.opcode = fnIndex;
            command.args[0] = arg;

#if debugInfo

            std::cout << "opcode:" << command.opcode << "\n";
            std::cout << "args:" << command.args[j] << "\n";

#endif

            bytecode.push_back(command);

            j++;

            for (int i = 0; i < bytecode.size(); i++) {
                switch (bytecode[i].opcode) {
                case 0:
                    std::cout << bytecode[i].args[0];
                    continue;
                case 1:
                    std::cout << "test complete!\n";
                    continue;
                case 2:
                    DrawText(bytecode[i].args[0].c_str(), 0, 0, 20, RED);
                    continue;
                default:
                    std::cout << "no\n";
                    continue;
                }
            }
        }

        //return bytecode;
    }
}