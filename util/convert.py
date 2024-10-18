def parse_usda_file(file_path):
    xform_list = []
    current_xform = None
    inside_xform = False

    with open(file_path, 'r') as file:
        while True:
            line = file.readline()
            if not line:
                break
            
            line = line.strip()

            # Detect the start of a new Xform block
            if line.startswith('def Xform'):
                # Get the Xform name
                xform_name = line.split('"')[1]

                # Ignore Xform if the name does not start with Cube, Cone, Cylinder, or Sphere
                if not (xform_name.startswith('Cube') or xform_name.startswith('Cone') or 
                        xform_name.startswith('Cylinder') or xform_name.startswith('Sphere')):
                    inside_xform = False  # Ignore this Xform block
                    current_xform = None  # Reset current Xform
                    continue
                
                # If valid, create a new Xform dictionary
                if current_xform:  # If an Xform was already being processed, add it to the list
                    xform_list.append(current_xform)
                current_xform = {'name': xform_name, 'rotateXYZ': None, 'scale': None, 'translate': None, 'mesh': None}
                inside_xform = True  # Mark that we're inside a valid Xform

            # Detect transformation operations within the Xform
            if inside_xform:
                if line.startswith('float3 xformOp:rotateXYZ'):
                    current_xform['rotateXYZ'] = line.split('=')[1].strip()
                elif line.startswith('float3 xformOp:scale'):
                    current_xform['scale'] = line.split('=')[1].strip()
                elif line.startswith('double3 xformOp:translate'):
                    current_xform['translate'] = line.split('=')[1].strip()
                
                # Detect the Mesh definition within the Xform
                if line.startswith('def Mesh'):
                    current_xform['mesh'] = line.split('"')[1]  # Get the Mesh name
                
                # Detect the end of the Xform block
                if line == '}':
                    inside_xform = False

    # Append the last Xform
    if current_xform:
        xform_list.append(current_xform)

    return xform_list


# Example usage
file_path = '/Users/qiangyuli/Blender/jumbo.usda'
xform_data = parse_usda_file(file_path)

# Print the extracted Xform information
for xform in xform_data:
    print(f"Xform Name: {xform['name']}")
    print(f"RotateXYZ: {xform['rotateXYZ']}")
    print(f"Scale: {xform['scale']}")
    print(f"Translate: {xform['translate']}")
    print(f"Mesh Name: {xform['mesh']}")
    print("---")

def format_xform_to_xml(xform_data):
    result_list = []

    for xform in xform_data:
        # Extract the scale values and format them
        scale_values = xform['scale'].strip('()').split(',')
        scale_x = float(scale_values[0].strip()) / 3
        scale_z = float(scale_values[1].strip()) / 3
        scale_y = float(scale_values[2].strip()) / 3
        scale_str = f'<scale x="{scale_x}" y="{scale_y}" z="{scale_z}"/>'

        # Extract the translate values and format them
        translate_values = xform['translate'].strip('()').split(',')
        translate_x = float(translate_values[0].strip()) / 3
        translate_z = float(translate_values[1].strip()) / 3
        translate_y = float(translate_values[2].strip()) / 3
        # Calculate new translate values as scale * (1 / translate)
        new_translate_x = translate_x * (1 / (scale_x * 2)) if translate_x != 0 else 0
        new_translate_z = translate_z * (1 / (scale_z * 2)) if translate_z != 0 else 0
        new_translate_y = translate_y * (1 / (scale_y * 2)) if translate_y != 0 else 0

        translate_str = f'<translate x="{new_translate_x }" y="{new_translate_y}" z="{new_translate_z}"/>'

        # Extract the rotation values and format multiple rotate blocks if necessary
        rotate_values = list(map(float, xform['rotateXYZ'].strip('()').split(',')))
        rotate_strs = []
        if rotate_values[0] != 0:
            rotate_strs.append(f'<rotate x="1" y="0" z="0" angle="{rotate_values[0]}"/>')
        if rotate_values[2] != 0:
            rotate_strs.append(f'<rotate x="0" y="1" z="0" angle="{rotate_values[2]}"/>')
        if rotate_values[1] != 0:
            rotate_strs.append(f'<rotate x="0" y="0" z="1" angle="{rotate_values[1]}"/>')

        # Extract the mesh name, take the part before the underscore, and convert to lowercase
        if xform['mesh']:
            mesh_name = xform['mesh'].split('_')[0].lower()
        else:
            mesh_name = 'unknown'

        object_str = f'<object type="primitive" name="{mesh_name}" >'
        diffuse_str = '<diffuse r="0" g="0" b="1"/>'

        # Combine all parts into the desired XML-like structure
        transblock_str = (
            '\t<transblock>\n'
            f'\t\t{scale_str}\n'
            f'\t\t{translate_str}\n'
            + ''.join([f'\t\t{rotate_str}\n' for rotate_str in rotate_strs]) +
            f'\t\t{object_str}\n'
            f'\t\t\t{diffuse_str}\n'
            '\t\t</object>\n'
            '\t</transblock>'
        )

        result_list.append(transblock_str)

    return result_list


def generate_scenefile(xform_data):
    # Scene structure header
    scene_header = '''
<scenefile>
    <globaldata>
        <diffusecoeff v="0.5"/>
        <specularcoeff v="0.5"/>
        <ambientcoeff v="0.5"/>
    </globaldata>

    <cameradata>
        <pos x="2" y="2" z="2"/>
        <focus x="0" y="0" z="0"/>
        <up x="0" y="1" z="0"/>
        <heightangle v="45"/>
    </cameradata>

    <lightdata>
        <id v="0"/>
        <color r="1" g="1" b="1"/>
        <position x="4" y="4" z="4"/>
    </lightdata>

    <lightdata>
        <id v="1"/>
        <color r="1" g="1" b="1"/>
        <position x="-3" y="3" z="3"/>
    </lightdata>

    <lightdata>
        <id v="2"/>
        <color r="1" g="1" b="1"/>
        <position x="-3" y="3" z="-3"/>
    </lightdata>

    <lightdata>
        <id v="3"/>
        <color r="1" g="1" b="1"/>
        <position x="3" y="3" z="-3"/>
    </lightdata>

    <object type="tree" name="root">
    '''
    
    # Scene structure footer
    scene_footer = '''
    </object>
</scenefile>
    '''

    # Generate the XML-like transblocks
    transblocks = format_xform_to_xml(xform_data)

    # Combine everything together
    scene_file = scene_header + '\n'.join(transblocks) + scene_footer

    return scene_file

def export_to_xml_file(xml_content, file_name):
    # Write the XML content to a file
    with open(file_name, 'w') as file:
        file.write(xml_content)

# Generate the final scenefile content
scenefile_content = generate_scenefile(xform_data)

# Specify the file name for the XML file
file_name = 'scene.xml'

# Export the content to an XML file
export_to_xml_file(scenefile_content, file_name)

print(f"Scene file has been written to {file_name}")