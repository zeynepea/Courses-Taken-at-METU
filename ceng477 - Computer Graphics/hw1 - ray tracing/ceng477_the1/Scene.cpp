#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"

using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void)
{
	for(int i = 0; i < cameras.size(); i++){
		Image image = Image(cameras[i]->imgPlane.nx, cameras[i]->imgPlane.ny);
		for(int x=0; x < cameras[i]->imgPlane.nx;x++){
			for(int y=0; y < cameras[i]->imgPlane.ny; y++){

				Ray ray = cameras[i]->getPrimaryRay(x, y);
				
				float pixel1 = 0,pixel2=0,pixel3=0;

				Vector3f pC = pixelColor(ray,maxRecursionDepth);
				//if(pC.x!=0&&pC.y!=0&&pC.z!=0)
				//printf("%f--%f--%f\n",pC.x,pC.y,pC.z );
				pixel1 += pC.x;
				pixel2 += pC.y;
				pixel3 += pC.z;
				if(check == false){
					//background
					unsigned char r,g,b;
					r = pScene->backgroundColor.x;
					g = pScene->backgroundColor.y;
					b = pScene->backgroundColor.z;
					Color color = {r,g,b};
					image.setPixelValue(x,y,color);
				}
				else{
					//intersect
					unsigned char r,g,b;
					r = pixel1 > 255 ? 255: round(pixel1);
					g = pixel2 > 255 ? 255: round(pixel2);
					b = pixel3 > 255 ? 255: round(pixel3);
					r = pixel1 < 0 ? 0: r;
					g = pixel2 < 0 ? 0: g;
					b = pixel3 < 0 ? 0: b;
					Color color = {r,g,b};
					image.setPixelValue(x,y,color);
				}
			}
		}
		image.saveImage(cameras[i]->imageName);	
	}
}

Vector3f Scene::ambientShading(int mat){
	Vector3f ambientShading;
	ambientShading.x = pScene->materials[mat]-> ambientRef.x * pScene->ambientLight.x;
	ambientShading.y = pScene->materials[mat]-> ambientRef.y * pScene->ambientLight.y;
	ambientShading.z = pScene->materials[mat]-> ambientRef.z * pScene->ambientLight.z;
	return ambientShading;
}

Vector3f Scene::diffuseShading(int hitObj, const Vector3f& w_i, const Vector3f& normal,const Vector3f& irr){
	float cos = w_i * normal;
	if(cos<0){
		cos=0;
	}
	Vector3f diff;
	int ind = objects[hitObj]->matIndex-1;
	diff.x = materials[ind]->diffuseRef.x * cos * irr.x;
	diff.y = materials[ind]->diffuseRef.y * cos * irr.y;
	diff.z = materials[ind]->diffuseRef.z * cos * irr.z;
	return diff;
}

Vector3f Scene::specularShading(int hitObj, const Vector3f& w_i, const Vector3f& normal,const Vector3f& w_o, const Vector3f& irr){
	Vector3f h;
	h = w_i + w_o;
	float h_len = sqrt(h*h);
	h = h/h_len;
	float cos = normal*h;
	if(cos<0){
		cos = 0;
	}
	Vector3f spec;
	int ind = objects[hitObj]->matIndex-1;
	spec.x = materials[ind]->specularRef.x * pow(cos, materials[ind]->phongExp) * irr.x;
	spec.y = materials[ind]->specularRef.y * pow(cos, materials[ind]->phongExp) * irr.y;
	spec.z = materials[ind]->specularRef.z * pow(cos, materials[ind]->phongExp) * irr.z;
	return spec;
}

Vector3f Scene::pixelColor(const Ray &ray, int maxDepth){

	Vector3f intersection;
	int hitObj;
	ReturnVal result;
	//bool check= false;
	//closest intersection point
	float tmin = INT8_MAX;
	float pixel1 = 0,pixel2=0,pixel3=0;

	for (int j = 0; j < objects.size(); j++){
		ReturnVal ret = objects[j]->intersect(ray);

		if (ret.hit == true){
			if (tmin > ray.gett(ret.int_point)){
				tmin = ray.gett(ret.int_point);
				intersection = ret.int_point;
				check =true;
				result = ret;
				hitObj = j;
				
				 //printf("%f-%f-%f  == %f-%f-%f\n", pixel1,pixel2,pixel3,pixel11,pixel21,pixel31);
			}
		}
		
	}
	if(result.hit==true){
		//for all point lights
		int deneme = objects[hitObj]->matIndex - 1;
				 			//printf("ever2\n");
		Vector3f ambient = ambientShading(deneme);
		pixel1 += ambient.x;
		pixel2 += ambient.y;
		pixel3 += ambient.z;
		for(int l = 0; l < lights.size(); l++){
			Vector3f irr = lights[l]->computeLightContribution(intersection);

			Vector3f w_i = lights[l]->position - intersection;
			float len_wi = sqrt(w_i*w_i);
			w_i = w_i / len_wi;

			float len_wo = sqrt(ray.direction * ray.direction);
			Vector3f w_o = -ray.direction;
			w_o = w_o / len_wo;

			//shadow ray
			Ray s = Ray(intersection, w_i);
			Vector3f eps = s.getPoint(this->shadowRayEps);
			Ray s_t = Ray(eps, w_i);

			//float light_t = sqrt((lights[l]->position - intersection) * (lights[l]->position - intersection));
			float light_t = s_t.gett(lights[l]->position);
			float lmin = light_t;
			//object front of light?
			float obj_d; 
			for(int o = 0; o < objects.size(); o++){
				ReturnVal ob = objects[o]->intersect(s_t);
				if((ob.hit == true)){
					obj_d = s_t.gett(ob.int_point);
					if(obj_d<lmin){
						lmin = obj_d;
						//printf("shadow\n");
					}
				}
			}
			//object is not on shadow
			if(light_t==lmin){
				//printf("seg0\n");
				Vector3f diffuse = diffuseShading(hitObj, w_i, result.normal, irr);
				//printf("seg1\n");
				Vector3f specular = specularShading(hitObj, w_i, result.normal, w_o, irr);
				//printf("seg2\n");
				pixel1 += diffuse.x + specular.x;
				pixel2 += diffuse.y + specular.y;
				pixel3 += diffuse.z + specular.z;

			}

		}
		bool mirror = (materials[deneme]->mirrorRef.x!=0.00)||(materials[deneme]->mirrorRef.y!=0.00)||(materials[deneme]->mirrorRef.z!=0.00);
		//int mirInd;
		//printf("%f\n",shadowRayEps );
		 
		//printf("%d\n",mirror );
		if(mirror&&(maxDepth>0)){
			//mi = hitObj;
			//mirror = hitObj;
			//hallo++;
			float len_wo = sqrt(ray.direction * ray.direction);
			Vector3f w_o = -ray.direction;
			w_o = w_o / len_wo;
			float nwo = result.normal * w_o;
			//Vector3f wr = ray.direction/sqrt(ray.direction*ray.direction) - result.normal * (result.normal * (ray.direction/sqrt(ray.direction*ray.direction)))*2; 
			Vector3f w_r = -w_o + result.normal * 2 * nwo;
			float len_wr = sqrt(w_r*w_r);
			w_r = w_r/len_wr;
			/*
			Ray mRay = Ray(result.int_point, w_r);
			Vector3f pp = mRay.getPoint(shadowRayEps);
			Ray mmRay = Ray(pp, w_r);*/
			Vector3f new_o = result.int_point + w_r * this->shadowRayEps;
			//new_o = new_o / sqrt(new_o*new_o);
			Ray mmRay = Ray(new_o, w_r);
			Vector3f mm = pixelColor(mmRay, maxDepth-1);
			pixel1 += materials[deneme]->mirrorRef.x*mm.x;
			pixel2 += materials[deneme]->mirrorRef.y*mm.y;
			pixel3 += materials[deneme]->mirrorRef.z*mm.z;

		}
	}
	
	Vector3f pixCol;
	pixCol.x = pixel1;
	pixCol.y = pixel2;
	pixCol.z = pixel3;
	return pixCol;
}
// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

