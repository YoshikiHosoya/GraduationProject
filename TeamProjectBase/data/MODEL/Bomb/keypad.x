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
 27;
 6.16698;5.50510;-3.76995;,
 7.06809;6.40622;-0.26184;,
 7.06809;-7.42119;-0.26184;,
 6.16698;-6.52008;-3.76995;,
 7.06809;6.40622;-0.26184;,
 -6.75931;6.40622;-0.26184;,
 -6.75931;-7.42119;-0.26184;,
 7.06809;-7.42119;-0.26184;,
 -6.75931;6.40622;-0.26184;,
 -5.85820;5.50510;-3.76995;,
 -5.85820;-6.52008;-3.76995;,
 -6.75931;-7.42119;-0.26184;,
 6.16698;5.50510;-3.76995;,
 -5.85820;5.50510;-3.76995;,
 -5.85820;-6.52008;-3.76995;,
 6.16698;-6.52008;-3.76995;,
 0.07624;-0.58030;-3.76959;,
 -5.85820;-6.52008;-3.76995;,
 -5.85820;5.50510;-3.76995;,
 6.16698;-6.52008;-3.76995;,
 6.16698;5.50510;-3.76995;,
 6.16242;3.43597;-3.61828;,
 -5.86275;3.43597;-3.61828;,
 6.13663;1.38717;-3.61828;,
 6.14953;-0.54665;-3.61828;,
 -5.87565;-0.54665;-3.61828;,
 -5.88854;1.38717;-3.61828;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,1,12,13;,
 4;14,15,2,11;,
 3;16,17,18;,
 3;16,19,17;,
 3;16,20,19;,
 4;12,21,22,13;,
 4;23,24,25,26;;
 
 MeshMaterialList {
  4;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.511200;0.448800;0.473600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.476800;0.740000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.367200;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.228800;0.228800;0.228800;0.450000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  12;
  -0.968558;0.000000;-0.248789;,
  0.968558;0.000000;-0.248790;,
  0.000000;0.000000;1.000000;,
  0.000001;0.000021;-1.000000;,
  0.000000;-0.073107;-0.997324;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.968557;-0.248791;,
  0.000000;-0.968558;-0.248789;,
  0.000031;0.000031;-1.000000;,
  0.000031;-0.036578;-0.999331;,
  -0.000030;0.000031;-1.000000;,
  -0.000030;-0.036578;-0.999331;;
  10;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  3;3,8,9;,
  3;3,10,8;,
  3;3,11,10;,
  4;11,4,4,9;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  27;
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
  0.493720;0.506280;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
