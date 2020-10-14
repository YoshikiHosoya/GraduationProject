xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 320;
 0.00000;12.02524;0.00000;,
 24.53796;12.02524;0.00000;,
 24.53796;11.18524;0.00000;,
 0.00000;11.18524;0.00000;,
 24.53796;12.02524;0.00000;,
 24.53796;12.02524;15.58574;,
 24.53796;11.18524;15.58574;,
 24.53796;11.18524;0.00000;,
 0.00000;12.02524;15.58574;,
 0.00000;12.02524;0.00000;,
 0.00000;11.18524;0.00000;,
 0.00000;11.18524;15.58574;,
 24.53796;12.02524;0.00000;,
 0.00000;12.02524;0.00000;,
 0.00000;11.18524;0.00000;,
 24.53796;11.18524;0.00000;,
 16.70151;11.07759;-4.24992;,
 24.07151;11.07759;-4.24992;,
 24.07151;9.64759;-4.24992;,
 16.70151;9.64759;-4.24992;,
 24.07151;11.07759;-4.24992;,
 24.07151;11.07759;-3.87992;,
 24.07151;9.64759;-3.87992;,
 24.07151;9.64759;-4.24992;,
 16.70151;11.07759;-3.87992;,
 16.70151;11.07759;-4.24992;,
 16.70151;9.64759;-4.24992;,
 16.70151;9.64759;-3.87992;,
 24.07151;11.07759;-4.24992;,
 16.70151;11.07759;-4.24992;,
 16.70151;9.64759;-4.24992;,
 24.07151;9.64759;-4.24992;,
 5.67360;11.24154;-0.09156;,
 16.48360;11.24154;-0.09156;,
 16.48360;10.31154;-0.09156;,
 5.67360;10.31154;-0.09156;,
 16.48360;11.24154;-0.09156;,
 16.48360;11.24154;0.27844;,
 16.48360;10.31154;0.27844;,
 16.48360;10.31154;-0.09156;,
 5.67360;11.24154;0.27844;,
 5.67360;11.24154;-0.09156;,
 5.67360;10.31154;-0.09156;,
 5.67360;10.31154;0.27844;,
 16.48360;11.24154;-0.09156;,
 5.67360;11.24154;-0.09156;,
 5.67360;10.31154;-0.09156;,
 16.48360;10.31154;-0.09156;,
 0.36016;11.10177;0.37455;,
 5.63017;11.10177;0.37455;,
 5.63017;6.46177;0.37455;,
 0.36016;6.46177;0.37455;,
 5.63017;11.10177;0.37455;,
 5.63017;11.10177;6.66455;,
 5.63017;6.46177;6.66455;,
 5.63017;6.46177;0.37455;,
 0.36016;11.10177;6.66455;,
 0.36016;11.10177;0.37455;,
 0.36016;6.46177;0.37455;,
 0.36016;6.46177;6.66455;,
 5.63017;11.10177;0.37455;,
 0.36016;11.10177;0.37455;,
 0.36016;6.46177;0.37455;,
 5.63017;6.46177;0.37455;,
 0.36016;6.58798;0.37455;,
 5.63017;6.58798;0.37455;,
 5.63017;1.43564;0.37455;,
 0.36016;1.43564;0.37455;,
 5.63017;6.58798;0.37455;,
 5.63017;6.58798;6.66455;,
 5.63017;1.43564;6.66455;,
 5.63017;1.43564;0.37455;,
 0.36016;6.58798;6.66455;,
 0.36016;6.58798;0.37455;,
 0.36016;1.43564;0.37455;,
 0.36016;1.43564;6.66455;,
 5.63017;6.58798;0.37455;,
 0.36016;6.58798;0.37455;,
 0.36016;1.43564;0.37455;,
 5.63017;1.43564;0.37455;,
 23.92398;11.44443;0.52392;,
 24.21398;11.44443;0.52392;,
 24.21398;0.25032;0.52392;,
 23.92398;0.25032;0.52392;,
 24.21398;11.44443;0.52392;,
 24.21398;11.44443;15.40793;,
 24.21398;0.25032;15.40793;,
 24.21398;0.25032;0.52392;,
 23.92398;11.44443;15.40793;,
 23.92398;11.44443;0.52392;,
 23.92398;0.25032;0.52392;,
 23.92398;0.25032;15.40793;,
 24.21398;11.44443;0.52392;,
 23.92398;11.44443;0.52392;,
 23.92398;0.25032;0.52392;,
 24.21398;0.25032;0.52392;,
 16.92428;10.91657;-4.13802;,
 17.21428;10.91657;-4.13802;,
 17.21428;9.72402;-4.13802;,
 16.92428;9.72402;-4.13802;,
 17.21428;10.91657;-4.13802;,
 17.21428;10.91657;2.43605;,
 17.21428;9.72402;2.43605;,
 17.21428;9.72402;-4.13802;,
 16.92428;10.91657;2.43605;,
 16.92428;10.91657;-4.13802;,
 16.92428;9.72402;-4.13802;,
 16.92428;9.72402;2.43605;,
 17.21428;10.91657;-4.13802;,
 16.92428;10.91657;-4.13802;,
 16.92428;9.72402;-4.13802;,
 17.21428;9.72402;-4.13802;,
 23.48070;10.91657;-4.13802;,
 23.77070;10.91657;-4.13802;,
 23.77070;9.72402;-4.13802;,
 23.48070;9.72402;-4.13802;,
 23.77070;10.91657;-4.13802;,
 23.77070;10.91657;2.43605;,
 23.77070;9.72402;2.43605;,
 23.77070;9.72402;-4.13802;,
 23.48070;10.91657;2.43605;,
 23.48070;10.91657;-4.13802;,
 23.48070;9.72402;-4.13802;,
 23.48070;9.72402;2.43605;,
 23.77070;10.91657;-4.13802;,
 23.48070;10.91657;-4.13802;,
 23.48070;9.72402;-4.13802;,
 23.77070;9.72402;-4.13802;,
 17.14294;9.83129;-3.97150;,
 23.52294;9.83129;-3.97150;,
 23.52294;9.61129;-3.97150;,
 17.14294;9.61129;-3.97150;,
 23.52294;9.83129;-3.97150;,
 23.52294;9.83129;3.03673;,
 23.52294;9.61129;3.03673;,
 23.52294;9.61129;-3.97150;,
 17.14294;9.83129;3.03673;,
 17.14294;9.83129;-3.97150;,
 17.14294;9.61129;-3.97150;,
 17.14294;9.61129;3.03673;,
 23.52294;9.83129;-3.97150;,
 17.14294;9.83129;-3.97150;,
 1.13351;1.40482;0.38460;,
 1.57192;1.40482;0.56620;,
 1.57192;0.14265;0.56620;,
 1.13351;0.14265;0.38460;,
 1.75351;1.40482;1.00461;,
 1.75351;0.14265;1.00461;,
 1.57192;1.40482;1.44301;,
 1.57192;0.14265;1.44301;,
 0.69510;1.40482;1.44301;,
 0.51351;1.40482;1.00461;,
 0.51351;0.14265;1.00461;,
 0.69510;0.14265;1.44301;,
 0.69510;1.40482;0.56620;,
 0.69510;0.14265;0.56620;,
 1.13351;1.40482;0.38460;,
 1.13351;0.14265;0.38460;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.62461;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.00461;,
 1.13351;1.40482;1.00461;,
 4.80694;1.40482;0.38460;,
 5.24534;1.40482;0.56620;,
 5.24534;0.14265;0.56620;,
 4.80694;0.14265;0.38460;,
 5.42694;1.40482;1.00461;,
 5.42694;0.14265;1.00461;,
 5.24534;1.40482;1.44301;,
 5.24534;0.14265;1.44301;,
 4.36853;1.40482;1.44301;,
 4.18694;1.40482;1.00461;,
 4.18694;0.14265;1.00461;,
 4.36853;0.14265;1.44301;,
 4.36853;1.40482;0.56620;,
 4.36853;0.14265;0.56620;,
 4.80694;1.40482;0.38460;,
 4.80694;0.14265;0.38460;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.62461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;1.00461;,
 4.80694;1.40482;5.22271;,
 5.24534;1.40482;5.40430;,
 5.24534;0.14265;5.40430;,
 4.80694;0.14265;5.22271;,
 5.42694;1.40482;5.84271;,
 5.42694;0.14265;5.84271;,
 5.24534;1.40482;6.28111;,
 5.24534;0.14265;6.28111;,
 4.36853;1.40482;6.28111;,
 4.18694;1.40482;5.84271;,
 4.18694;0.14265;5.84271;,
 4.36853;0.14265;6.28111;,
 4.36853;1.40482;5.40430;,
 4.36853;0.14265;5.40430;,
 4.80694;1.40482;5.22271;,
 4.80694;0.14265;5.22271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;6.46271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;5.84271;,
 4.80694;1.40482;5.84271;,
 1.09227;1.40482;5.23836;,
 1.53067;1.40482;5.41996;,
 1.53067;0.14265;5.41996;,
 1.09227;0.14265;5.23836;,
 1.71227;1.40482;5.85836;,
 1.71227;0.14265;5.85836;,
 1.53067;1.40482;6.29677;,
 1.53067;0.14265;6.29677;,
 0.65386;1.40482;6.29677;,
 0.47227;1.40482;5.85836;,
 0.47227;0.14265;5.85836;,
 0.65386;0.14265;6.29677;,
 0.65386;1.40482;5.41996;,
 0.65386;0.14265;5.41996;,
 1.09227;1.40482;5.23836;,
 1.09227;0.14265;5.23836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;6.47836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;5.85836;,
 1.09227;1.40482;5.85836;,
 17.06878;11.12960;2.89398;,
 23.48993;11.12960;2.89398;,
 23.48993;9.75175;2.89398;,
 17.06878;9.75175;2.89398;,
 23.48993;11.12960;2.89398;,
 23.48993;11.12960;4.53398;,
 23.48993;9.75175;4.53398;,
 23.48993;9.75175;2.89398;,
 17.06878;11.12960;4.53398;,
 17.06878;11.12960;2.89398;,
 17.06878;9.75175;2.89398;,
 17.06878;9.75175;4.53398;,
 23.48993;11.12960;2.89398;,
 17.06878;11.12960;2.89398;,
 17.06878;9.75175;2.89398;,
 23.48993;9.75175;2.89398;,
 0.39054;6.64579;0.36555;,
 5.56054;6.64579;0.36555;,
 5.56054;6.42579;0.36555;,
 0.39054;6.42579;0.36555;,
 5.56054;6.64579;0.36555;,
 5.56054;6.64579;6.64555;,
 5.56054;6.42579;6.64555;,
 5.56054;6.42579;0.36555;,
 0.39054;6.64579;6.64555;,
 0.39054;6.64579;0.36555;,
 0.39054;6.42579;0.36555;,
 0.39054;6.42579;6.64555;,
 24.06106;0.26715;0.68206;,
 24.16005;0.26715;0.72307;,
 24.16005;0.11715;0.72307;,
 24.06106;0.11715;0.68206;,
 24.20106;0.26715;0.82206;,
 24.20106;0.11715;0.82206;,
 24.16005;0.26715;0.92106;,
 24.16005;0.11715;0.92106;,
 23.96206;0.26715;0.92106;,
 23.92106;0.26715;0.82206;,
 23.92106;0.11715;0.82206;,
 23.96206;0.11715;0.92106;,
 23.96206;0.26715;0.72307;,
 23.96206;0.11715;0.72307;,
 24.06106;0.26715;0.68206;,
 24.06106;0.11715;0.68206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.96206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;0.82206;,
 24.06106;0.26715;14.88379;,
 24.16005;0.26715;14.92480;,
 24.16005;0.11715;14.92480;,
 24.06106;0.11715;14.88379;,
 24.20106;0.26715;15.02379;,
 24.20106;0.11715;15.02379;,
 24.16005;0.26715;15.12279;,
 24.16005;0.11715;15.12279;,
 23.96206;0.26715;15.12279;,
 23.92106;0.26715;15.02379;,
 23.92106;0.11715;15.02379;,
 23.96206;0.11715;15.12279;,
 23.96206;0.26715;14.92480;,
 23.96206;0.11715;14.92480;,
 24.06106;0.26715;14.88379;,
 24.06106;0.11715;14.88379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.16379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.02379;,
 24.06106;0.26715;15.02379;;
 
 136;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,5,12,13;,
 4;14,15,6,11;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;24,21,28,29;,
 4;30,31,22,27;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;40,37,44,45;,
 4;46,47,38,43;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;56,57,58,59;,
 4;56,53,60,61;,
 4;62,63,54,59;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;72,69,76,77;,
 4;78,79,70,75;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;88,85,92,93;,
 4;94,95,86,91;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;104,101,108,109;,
 4;110,111,102,107;,
 4;112,113,114,115;,
 4;116,117,118,119;,
 4;120,121,122,123;,
 4;120,117,124,125;,
 4;126,127,118,123;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;136,133,140,141;,
 4;142,143,144,145;,
 4;143,146,147,144;,
 4;146,148,149,147;,
 4;150,151,152,153;,
 4;151,154,155,152;,
 4;154,156,157,155;,
 3;158,143,142;,
 3;159,146,143;,
 3;160,148,146;,
 3;161,162,148;,
 3;163,150,162;,
 3;164,151,150;,
 3;165,154,151;,
 3;166,156,154;,
 4;167,168,169,170;,
 4;168,171,172,169;,
 4;171,173,174,172;,
 4;175,176,177,178;,
 4;176,179,180,177;,
 4;179,181,182,180;,
 3;183,168,167;,
 3;184,171,168;,
 3;185,173,171;,
 3;186,187,173;,
 3;188,175,187;,
 3;189,176,175;,
 3;190,179,176;,
 3;191,181,179;,
 4;192,193,194,195;,
 4;193,196,197,194;,
 4;196,198,199,197;,
 4;200,201,202,203;,
 4;201,204,205,202;,
 4;204,206,207,205;,
 3;208,193,192;,
 3;209,196,193;,
 3;210,198,196;,
 3;211,212,198;,
 3;213,200,212;,
 3;214,201,200;,
 3;215,204,201;,
 3;216,206,204;,
 4;217,218,219,220;,
 4;218,221,222,219;,
 4;221,223,224,222;,
 4;225,226,227,228;,
 4;226,229,230,227;,
 4;229,231,232,230;,
 3;233,218,217;,
 3;234,221,218;,
 3;235,223,221;,
 3;236,237,223;,
 3;238,225,237;,
 3;239,226,225;,
 3;240,229,226;,
 3;241,231,229;,
 4;242,243,244,245;,
 4;246,247,248,249;,
 4;250,251,252,253;,
 4;250,247,254,255;,
 4;256,257,248,253;,
 4;258,259,260,261;,
 4;262,263,264,265;,
 4;266,267,268,269;,
 4;270,271,272,273;,
 4;271,274,275,272;,
 4;274,276,277,275;,
 4;278,279,280,281;,
 4;279,282,283,280;,
 4;282,284,285,283;,
 3;286,271,270;,
 3;287,274,271;,
 3;288,276,274;,
 3;289,290,276;,
 3;291,278,290;,
 3;292,279,278;,
 3;293,282,279;,
 3;294,284,282;,
 4;295,296,297,298;,
 4;296,299,300,297;,
 4;299,301,302,300;,
 4;303,304,305,306;,
 4;304,307,308,305;,
 4;307,309,310,308;,
 3;311,296,295;,
 3;312,299,296;,
 3;313,301,299;,
 3;314,315,301;,
 3;316,303,315;,
 3;317,304,303;,
 3;318,307,304;,
 3;319,309,307;;
 
 MeshMaterialList {
  6;
  136;
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  5,
  5,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.680800;0.514400;0.341600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.380000;0.380000;0.380000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.106400;0.075200;0.075200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.746400;0.671200;0.395200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.501600;0.501600;0.501600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  95;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.707114;0.000000;-0.707100;,
  1.000000;0.000000;0.000004;,
  0.923880;0.000000;0.382681;,
  -0.923880;0.000000;0.382681;,
  -1.000000;0.000000;0.000004;,
  -0.707114;0.000000;-0.707100;,
  0.000000;1.000000;0.000000;,
  0.000004;0.000000;-1.000000;,
  0.707110;0.000000;-0.707104;,
  1.000000;0.000000;0.000004;,
  0.923873;0.000000;0.382700;,
  -0.923881;0.000000;0.382681;,
  -1.000000;0.000000;0.000004;,
  -0.707114;0.000000;-0.707100;,
  0.000000;1.000000;0.000000;,
  0.000004;0.000000;-1.000000;,
  0.707103;0.000000;-0.707111;,
  1.000000;0.000000;0.000004;,
  0.923873;0.000000;0.382700;,
  -0.923881;0.000000;0.382681;,
  -1.000000;0.000000;0.000004;,
  -0.707107;0.000000;-0.707107;,
  0.000000;1.000000;0.000000;,
  0.000004;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;-0.000004;,
  0.923876;0.000000;0.382692;,
  -0.923884;0.000000;0.382674;,
  -1.000000;0.000000;-0.000004;,
  -0.707111;0.000000;-0.707103;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000016;0.000000;-1.000000;,
  0.707104;0.000000;-0.707110;,
  1.000000;0.000000;-0.000018;,
  0.923867;0.000000;0.382715;,
  -0.923902;0.000000;0.382630;,
  -1.000000;0.000000;-0.000018;,
  -0.707125;0.000000;-0.707089;,
  0.000000;1.000000;0.000000;,
  0.000016;0.000000;-1.000000;,
  0.707102;0.000000;-0.707111;,
  1.000000;0.000000;-0.000019;,
  0.923867;0.000000;0.382713;,
  -0.923903;0.000000;0.382628;,
  -1.000000;0.000000;-0.000019;,
  -0.707123;0.000000;-0.707090;;
  136;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;24,24,24,24;,
  4;25,25,25,25;,
  4;26,26,26,26;,
  4;27,27,27,27;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;30,30,30,30;,
  4;31,31,31,31;,
  4;32,32,32,32;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;30,30,30,30;,
  4;31,31,31,31;,
  4;32,32,32,32;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;40,41,41,40;,
  4;41,42,42,41;,
  4;42,43,43,42;,
  4;44,45,45,44;,
  4;45,46,46,45;,
  4;46,40,40,46;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  3;39,39,39;,
  4;48,49,49,48;,
  4;49,50,50,49;,
  4;50,51,51,50;,
  4;52,53,53,52;,
  4;53,54,54,53;,
  4;54,48,48,54;,
  3;47,47,47;,
  3;47,47,47;,
  3;47,47,47;,
  3;47,47,47;,
  3;47,47,47;,
  3;47,47,47;,
  3;47,47,47;,
  3;47,47,47;,
  4;56,57,57,56;,
  4;57,58,58,57;,
  4;58,59,59,58;,
  4;60,61,61,60;,
  4;61,62,62,61;,
  4;62,56,56,62;,
  3;55,55,55;,
  3;55,55,55;,
  3;55,55,55;,
  3;55,55,55;,
  3;55,55,55;,
  3;55,55,55;,
  3;55,55,55;,
  3;55,55,55;,
  4;64,65,65,64;,
  4;65,66,66,65;,
  4;66,67,67,66;,
  4;68,69,69,68;,
  4;69,70,70,69;,
  4;70,64,64,70;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  4;71,71,71,71;,
  4;72,72,72,72;,
  4;73,73,73,73;,
  4;74,74,74,74;,
  4;75,75,75,75;,
  4;76,76,76,76;,
  4;77,77,77,77;,
  4;78,78,78,78;,
  4;80,81,81,80;,
  4;81,82,82,81;,
  4;82,83,83,82;,
  4;84,85,85,84;,
  4;85,86,86,85;,
  4;86,80,80,86;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  3;79,79,79;,
  4;88,89,89,88;,
  4;89,90,90,89;,
  4;90,91,91,90;,
  4;92,93,93,92;,
  4;93,94,94,93;,
  4;94,88,88,94;,
  3;87,87,87;,
  3;87,87,87;,
  3;87,87,87;,
  3;87,87,87;,
  3;87,87,87;,
  3;87,87,87;,
  3;87,87,87;,
  3;87,87,87;;
 }
 MeshTextureCoords {
  320;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;;
 }
}
