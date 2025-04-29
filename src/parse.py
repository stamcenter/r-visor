import sys

class Inst_Type:
    def __init__(self, name, length, components):
        self.name = name
        self.length = length
        self.components = components

class Component:
    def __init__(self, start, width, signed):
        self.start = start
        self.width = width
        self.signed = signed

class Compound_Component:
    def __init__(self, signed):
        self.components = []
        self.signed = signed

class Sub_Component:
    def __init__(self, start, width, pos_start, pos_width, pos_end):
        self.start = start
        self.width = width
        self.pos_start = pos_start
        self.pos_width = pos_width
        self.pos_end =  pos_end

class Instruction:
    def __init__(self):
        self.components = {}

def parse(filename):
    lines = []
    with open(filename) as f:
        lines = f.read().replace("\n","").replace("\t","").split("}")

    instruction_types = {}
    instructions = []

    for line in lines:
        if line.startswith("//"):
            continue
        elif line.startswith("InstType"):
            compound = False
            length = 0
            type_components = {}
            
            line = line.replace("InstType", "")
            type_name, components = line.split("{")

            for component in components.split(","):
                signed = False
                compound_component = False
                sub_component_start = 0
                sub_component_width = 0
                component = component.strip()
                comps = component.split(" ")
                component_name = comps[0]
                range = comps[1]
                if len(comps) > 2 and "-s" in comps[2]:
                    range = range.replace("-s", "").strip()
                    signed = True
                    # print("found signed")


                # Handling compound
                if "(" in component_name and ")" in component_name:
                    compound_component = True
                    component_name, component_parts = component_name.split("(")
                    component_parts = component_parts.replace(")", "")

                    if ":" in component_parts:
                        sub_component_start, sub_component_end = component_parts.split(":")
                        sub_component_start = int(sub_component_start)
                        sub_component_end = int(sub_component_end)
                    else:
                        sub_component_start = int(component_parts)
                        sub_component_end = sub_component_start
                    sub_component_width = sub_component_end - sub_component_start + 1

                # Handling range
                end = 0
                range = range.replace("[", "").replace("]", "")
                if ":" in range:
                    start, end = range.split(":")
                    start = int(start)
                    end = int(end)
                    width = end - start +1
                else:
                    start = int(range)
                    end = start+1
                    width = 1
                length = max(end, length)

                if compound_component:
                    if type_components.get(component_name):
                        type_components[component_name].components.append(Sub_Component(start, width, sub_component_start, sub_component_width, sub_component_end))
                    else:
                        type_components[component_name] = Compound_Component(signed=signed)
                        type_components[component_name].components.append(Sub_Component(start, width, sub_component_start, sub_component_width, sub_component_end))

                else:
                    type_components[component_name] = Component(start, width, signed=signed)



            new_type = Inst_Type(type_name.strip(), length, type_components)
            # instruction_types.append(new_type)
            instruction_types[type_name.strip()] = new_type


        elif line.startswith("Instruction"):

            line = line.replace("Instruction", "")
            inst_name, components = line.split("{")
            # print(inst_name)

            instruction_data = {"name":inst_name.strip()}
            for component in components.split(","):
                component = component.strip()
                component_name, value = component.split(" ")

                instruction_data[component_name] = value
            instructions.append(instruction_data)


            

        # elif line.startswith("#"):
        #     continue

    keys = []
    for instruction_type in instruction_types.values():
        for comp in instruction_type.components.keys():
            keys.append(comp)

    keys = list(set(keys))

    # Creating map for opcodes and opfunct3s
    opcodes = {}
    opfunct3s = {}
    groups = []

    inst_names_file = open("instruction_names.h", "w")

    inst_names_file.write("#ifndef INSTRUCTION_NAMES_H\n#define INSTRUCTION_NAMES_H\n\n")
    # inst_names_file.write("namespace rail{\n")
    inst_names_file.write("typedef enum{\n")

    for inst in instructions:
        name = inst["name"].replace(".", "_")
        inst_names_file.write(f"{name},\n")
        if inst.get("opcode") and not opcodes.get(inst['opcode']):
            opcodes[inst['opcode']] = (inst['type'])

        if inst.get("opfunct3") and not opfunct3s.get(inst['opfunct3']):
            opfunct3s[inst['opfunct3']] = (inst['type'])

        if inst.get("group") and inst["group"] not in groups:
            group = inst["group"]
            groups.append(inst["group"])        

    inst_names_file.write("}InstName;\n")

    inst_names_file.write("\nstatic const char *InstNameString[] = {\n")
    for inst in instructions:
        name = inst["name"].replace(".", "_")
        inst_names_file.write(f"\"{name}\",\n")
    inst_names_file.write("};\n")
    inst_names_file.write("#endif\n")

    # with open("test.cpp", "w") as f:
    decode_file = open("decode.c", "w")
    instructions_file = open("instructions.h", "w")
    inst_names_file = open("instruction_names.h", "w")
    decode_headerfile = open("decode.h", "w")

    decode_headerfile.write("#ifndef DECODE_H\n#define DECODE_H\n\n")
    decode_headerfile.write("#include <stdint.h>\n")
    decode_headerfile.write("#include <stdio.h>\n\n")
    decode_headerfile.write("#include \"printUtils.h\"\n")
    decode_headerfile.write("#include \"instructions.h\"\n\n")

    instructions_file.write("#ifndef INSTRUCTIONS_H\n#define INSTRUCTIONS_H\n")
    instructions_file.write("#include <stdint.h>\n#include \"instruction_names.h\"\n")


    encoderFile = open("encode.h", "w")

    # encoderHeader = open("encode.h", "w")
    encoderFile.write("#ifndef ENCODE_H\n#define ENCODE_H\n")
    encoderFile.write("#include <stdio.h>\n")
    encoderFile.write("#include <stdint.h>\n")
    encoderFile.write("#include \"decode.h\"\n")
    encoderFile.write("#include \"instruction_names.h\"\n")

    # encoderFile.write("#include \"encode.h\"\n")
    # encoderFile.write("namespace rail{\n")

    for inst_type in instruction_types.values():
        
        encoderFile.write(f"uint32_t inline encode_{inst_type.name}type(")
        encoderArgs = []
        for key in inst_type.components.keys():
            encoderArgs.append(f"int {key}")
        encoderArgs = ", ".join(encoderArgs)
        encoderFile.write(f"{encoderArgs}")
        encoderFile.write("){\n")
        encoderFile.write("uint32_t instruction = ")

        for component_name, component in inst_type.components.items():
            
            if type(component) == Compound_Component:
                for subcomp in component.components:
                    mask = "0b" + "1"*subcomp.pos_width
                    encoderFile.write(f"((({component_name} >> {subcomp.pos_start}) & {mask}) << {subcomp.start}) | ")
                pass
            else:
                mask = "0b" + "1"*component.width
                encoderFile.write(f"(({component_name} & {mask}) << {component.start}) | ")

        encoderFile.write("0")
        encoderFile.write(";\nreturn instruction;\n}\n\n")


    # Writing Encoding Function
    # encoderFile.write(f"\nuint32_t encode_instruction(InstName inst, int arg1, int arg2, int arg3, int arg4, int arg5){{\n")
    # encoderFile.write(f"\nuint32_t encode_instruction(InstName inst, int arg1, int arg2, int arg3){{\n")
    # encoderFile.write("switch(inst){\n")
    for inst in instructions:
        name = inst["name"].replace(".", "_")
        enc_params = []
        func_params = []
        inst_type = inst["type"]
        
        x = 1
        
        for comp in instruction_types[inst_type].components.keys():
            if not inst.get(comp):
                enc_params.append(comp)
                func_params.append(f"int {comp}")
                x+=1
            else:
                enc_params.append(inst.get(comp))

        enc_params = ", ".join(enc_params)
        func_params = ", ".join(func_params)
        
        encoderFile.write(f"\nuint32_t inline encode_{name}({func_params}){{\n")

        # encoderFile.write(f"case {name} :\n")
        encoderFile.write(f"\treturn encode_{inst_type}type({enc_params});\n}}\n")
        # encoderFile.write("}\n")
        # encoderFile.write("break;\n")




    # Writing struct for inst
    instructions_file.write("typedef enum {")
    for instruction_type in instruction_types.values():
        instructions_file.write(f"{instruction_type.name}_TYPE,")
    instructions_file.write("UNKNOWN_TYPE} InstType;\n\n")

    # Creating Opcodes map
    # instructions_file.write("static std::unordered_map<int, InstType> opcodesMap{\n")
    # for opcode, inst_type in opcodes.items():
    #     instructions_file.write(f"{{ {opcode}, {inst_type}_TYPE }},\n")
    # instructions_file.write("};\n\n")

    # instructions_file.write("static std::unordered_map<int, InstType> opfunctsMap{\n")
    # for opcode, inst_type in opfunct3s.items():
    #     instructions_file.write(f"{{ {opcode}, {inst_type}_TYPE }},\n")
    # instructions_file.write("};\n\n")

    instructions_file.write("static InstType getOpcodeType(int opcode){\nswitch(opcode){\n")
    for opcode, inst_type in opcodes.items():
        instructions_file.write(f"case {opcode}: return {inst_type}_TYPE; \n")
    instructions_file.write("default: return UNKNOWN_TYPE;\n")
    instructions_file.write("}\n};\n\n")

    instructions_file.write("static InstType getOpfunctType (int funct){\nswitch(funct){\n")
    for opcode, inst_type in opfunct3s.items():
        instructions_file.write(f"case {opcode}: return {inst_type}_TYPE; \n")
    instructions_file.write("default: return UNKNOWN_TYPE;\n")
    instructions_file.write("}\n};\n\n")


    # Creating Struct for Instruction
    instructions_file.write("typedef struct{\n")
    for key in keys:
        instructions_file.write(f"\tint32_t {key};\n")
    instructions_file.write("\tuint64_t group;\n")
    instructions_file.write("\tuint64_t address;\n")
    instructions_file.write("\tInstName name;\n")
    instructions_file.write("\tInstType type;\n")
    instructions_file.write("}RvInst;\n\n")

    instructions_file.write("typedef enum{\n")
    instructions_file.write(f"\tNO_GROUP,\n")
    for group in groups:
        instructions_file.write(f"\t{group}_GROUP,\n")
    instructions_file.write("}RvInstGroup;\n\n")

    instructions_file.write("#endif")

    # Creating cpp file
    decode_file.write("#include \"decode.h\"\n\n")
    decode_file.write("#include \"instruction_names.h\"\n")
    # decode_file.write("namespace rail{\n")

    # Creating logic for instruction decoding
    for instruction_type in instruction_types.values():
        decode_headerfile.write(f"RvInst decode_{instruction_type.name}type(uint32_t instruction);\n")
        
        decode_file.write(f"RvInst decode_{instruction_type.name}type(uint32_t instruction){{\n")
        decode_file.write("\tRvInst decodedInst;\n")
        decode_file.write(f"\tdecodedInst.type = {instruction_type.name}_TYPE;\n")

        # Adding Decoding logic for each component
        for comp_name, comp in instruction_type.components.items():
            # Adding decoding logic for unsplit components
            full_width = 0
            if type(comp) is Component:
                mask = "0b" + "1"*comp.width
                # if "imm" not in comp_name:
                if not comp.signed:
                    decode_file.write(f"\tdecodedInst.{comp_name} = (instruction >> {comp.start}) & {mask};\n")
                else:
                    decode_file.write(f"\tint32_t {comp_name} = (instruction >> {comp.start}) & {mask};\n")
                full_width = comp.width
            # Adding decoding logic for split components
            elif type(comp) is Compound_Component:
                final_comp = []
                
                for subcomp in comp.components:
                    inst_mask = "0b" + "1"*subcomp.width
                    if subcomp.pos_end > full_width:
                        full_width = subcomp.pos_end
                    # full_width += subcomp.width
                    # print(f"{comp_name} {subcomp.pos_start} {subcomp.pos_width} {subcomp.start} {subcomp.width}")
                    decode_file.write(f"\tint {comp_name}_{subcomp.pos_start} = (instruction >> {subcomp.start}) & {inst_mask};\n")

                    final_comp.append(f"({comp_name}_{subcomp.pos_start} << {subcomp.pos_start})")
                    final_comp_str = "|".join(final_comp)
                decode_file.write(f"\tint {comp_name} = {final_comp_str};\n")

            # Logic for sign extension
            if comp.signed:
                sign_extend_mask = "0b" + "1"*(32 - full_width) + "0"*full_width
                sign_extend_check = "0b" + "1" + "0"*(full_width -1)
                # decode_file.write(f"\t{comp_name} = {comp_name} >> {full_width} == 0b1 ? {comp_name} | {sign_extend_mask} : {comp_name};\n")
                decode_file.write(f"\t{comp_name} = ({comp_name} >> {full_width}) ? {comp_name} | {sign_extend_mask} : {comp_name};\n")
            if type(comp) is Compound_Component or comp.signed:
                decode_file.write(f"\tdecodedInst.{comp_name} = {comp_name};\n")
                # print(full_width)
        decode_file.write("\treturn decodedInst;\n")
        decode_file.write("}\n\n")

    # Creating function for decoding 32 bit instructions
    decode_headerfile.write("RvInst decode_instruction32(uint32_t instruction);\n")
    decode_file.write("RvInst decode_instruction32(uint32_t instruction){\n")
    decode_file.write("RvInst decodedInst;\n")

    # Creating switch for opcode
    decode_file.write("int opcode = instruction & 0b1111111;\n")
    decode_file.write("switch(opcode){\n")
    for opcode, inst_type in opcodes.items():
        decode_file.write(f"\tcase {opcode}:\n")
        # for instType in instruction_types:
        decode_file.write(f"\t\tdecodedInst = decode_{inst_type}type(instruction);\n")
        
        # Inserting individual instructions under correct opcode in decoder 
        for instruction in instructions:
            if instruction.get("opcode") and instruction["opcode"] == opcode:
                name = instruction.get("name").replace(".","_")
                # print(f"Type {instType.name} - {name}")

                # Composing conditional statement to determine instruction based on fields
                decode_file.write(f"\t\tif(")

                added_fields = 0
                size = len(instruction.items())

                off = 1
                if instruction.get("group"):
                    off = 2
                for index, (field, value) in enumerate(instruction.items()):
                    if field != "type" and field != "name" and field != "opcode" and field != "group":
                        decode_file.write(f"decodedInst.{field} == {value} ")
                        added_fields +=1
                        if index < size-off:
                            decode_file.write("&& ")
                if added_fields == 0:
                    decode_file.write("1")
                decode_file.write(")")
                if instruction.get("group"):
                    group = instruction["group"]
                    decode_file.write(f"{{\n\t\t\tdecodedInst.group = {group}_GROUP;\n")
                    decode_file.write(f"\t\t\tdecodedInst.name = {name};\n\t\t\treturn decodedInst;\n\t\t}}\n")
                else:
                    decode_file.write(f"{{\n\t\t\tdecodedInst.group = NO_GROUP;\n")
                    decode_file.write(f"\t\t\tdecodedInst.name = {name};\n\t\t\treturn decodedInst;\n\t\t}}\n")
                    # decode_file.write(f"{{\n\t\t\tdecodedInst.name = \"{name}\";\n\t\t\treturn decodedInst;\n\t\t}}\n")
        decode_file.write("\t\tbreak;\n")
    decode_file.write("\tdefault: \n\t\treturn decodedInst;\n")
    decode_file.write("};\n")
    decode_file.write("return decodedInst;\n")
    decode_file.write("}\n\n")

    # Decoding Compressed Instructions
    decode_headerfile.write("RvInst decode_instruction16(uint32_t instruction);\n")
    decode_file.write("RvInst decode_instruction16(uint32_t instruction){\n")
    decode_file.write("RvInst decodedInst;\n")

    decode_file.write("int opfunct3 = ((instruction & 0b11) << 3) | (instruction >> 13);\n")
    decode_file.write("switch(opfunct3){\n")
    for opfunct3, inst_type in opfunct3s.items():
        decode_file.write(f"\tcase {opfunct3}:\n")
        # for instType in instruction_types:
        decode_file.write(f"\t\tdecodedInst = decode_{inst_type}type(instruction);\n")
        
        # Inserting individual instructions under correct opcode in decoder 
        for instruction in instructions:
            if instruction.get("opfunct3") and instruction["opfunct3"] == opfunct3:
                name = instruction.get("name").replace(".","_")
                # Composing conditional statement to determine instruction based on fields
                decode_file.write(f"\t\tif(")

                added_fields = 0
                size = len(instruction.items())
                off = 1
                if instruction.get("group"):
                    off = 2
                for index, (field, value) in enumerate(instruction.items()):
                    if field != "type" and field != "name" and field != "opfunct3" and field != "group":
                        decode_file.write(f"decodedInst.{field} == {value} ")
                        added_fields +=1
                        if index < size-off:
                            decode_file.write("&& ")
                if added_fields == 0:
                    decode_file.write("1")
                decode_file.write(")")
                if instruction.get("group"):
                    group = instruction["group"]
                    decode_file.write(f"{{\n\t\t\tdecodedInst.group = {group}_GROUP;\n")
                    decode_file.write(f"\t\t\tdecodedInst.name = {name};\n\t\t\treturn decodedInst;\n\t\t}}\n")
                else:
                    decode_file.write(f"{{\n\t\t\tdecodedInst.group = NO_GROUP;\n")
                    decode_file.write(f"\t\t\tdecodedInst.name = {name};\n\t\t\treturn decodedInst;\n\t\t}}\n")
        decode_file.write("\t\tbreak;\n")
    decode_file.write("\tdefault: \n\t\treturn decodedInst;\n")
    decode_file.write("};\n")

    decode_file.write("return decodedInst;\n")
    decode_file.write("}\n\n")

    # Creating main function for decoding instruction based on length
    decode_headerfile.write("RvInst decode_instruction(uint32_t instruction, int debug);\n")
    decode_file.write("RvInst decode_instruction(uint32_t instruction, int debug){\n")
    decode_file.write("RvInst decodedInst;\n")
    decode_file.write("uint32_t instType = instruction & 0b11;\n")
    decode_file.write("if(instType == 0b11) \n")
    decode_file.write("decodedInst = decode_instruction32(instruction);\n")
    decode_file.write("else \n")
    decode_file.write("decodedInst = decode_instruction16(instruction);\n")
    decode_file.write("if(debug){\n")
    decode_file.write("printInstruction(decodedInst);\n")
    decode_file.write("}\n")
    decode_file.write("return decodedInst;\n")
    # decode_file.write("}\n\n")
    decode_file.write("}\n\n")

    # decode_headerfile.write("}\n\n")
    decode_headerfile.write("#endif\n\n")

    encoderFile.write("#endif\n\n")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python parse.py <filename>")
        sys.exit(1)
    filename = sys.argv[1]
    parse(filename)
    print("CodeGen complete")
