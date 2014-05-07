/*
// Point Cloud Library includes
#include <pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

#include "greedysurface.h"

namespace Degenerator
{

void greedySurface(std::vector<Ogre::Vector3>& pointCloud, AStruct& resultStruct)
{

  // Load input file into a PointCloud<T> with an appropriate type
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  //cloud->
  //sensor_msgs::PointCloud2 cloud_blob;
  //pcl::io::loadPCDFile("bun0.pcd", cloud_blob);
  //pcl::fromROSMsg(cloud_blob, *cloud);
  //* the data should be available in cloud
  for(int i = 0; i < pointCloud.size(); ++i)
  {
      cloud->push_back(pcl::PointXYZ(pointCloud.at(i).x, pointCloud.at(i).y, pointCloud.at(i).z));
  }

  // Normal estimation*
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
  pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
  tree->setInputCloud(cloud);
  n.setInputCloud(cloud);
  n.setSearchMethod(tree);
  n.setKSearch(20);
  n.compute(*normals);
  //* normals should not contain the point normals + surface curvatures

  // Concatenate the XYZ and normal fields*
  pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
  pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);
  //* cloud_with_normals = cloud + normals

  // Create search tree*
  pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
  tree2->setInputCloud(cloud_with_normals);

  // Initialize objects
  pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
  pcl::PolygonMesh triangles;

  // Set the maximum distance between connected points (maximum edge length)
  //gp3.setSearchRadius(0.025);
  gp3.setSearchRadius(10);

  // Set typical values for the parameters
  gp3.setMu(2.5);
  gp3.setMaximumNearestNeighbors(300);
  gp3.setMaximumSurfaceAngle(M_PI/4); // 45 degrees
  gp3.setMinimumAngle(M_PI/18); // 10 degrees
  gp3.setMaximumAngle(2*M_PI/3); // 120 degrees
  gp3.setNormalConsistency(false);

  // Get result
  gp3.setInputCloud(cloud_with_normals);
  gp3.setSearchMethod(tree2);
  gp3.reconstruct(triangles);

  // Additional vertex information
  std::vector<int> parts = gp3.getPartIDs();
  std::vector<int> states = gp3.getPointStates();

  int indices = 0;
  pcl::PointCloud<pcl::PointXYZ> newCloud;
  pcl::fromROSMsg(triangles.cloud, newCloud);

  for(int i = 0; i < triangles.polygons.size(); ++i)
  {
      resultStruct.indices.push_back(indices);
      ++indices;
      resultStruct.indices.push_back(indices);
      ++indices;
      resultStruct.indices.push_back(indices);
      ++indices;

      Ogre::Vector3 v0, v1, v2;

      v0 = Ogre::Vector3(
                  newCloud.points.at(triangles.polygons[i].vertices[0]).x,
                  newCloud.points.at(triangles.polygons[i].vertices[0]).y,
                  newCloud.points.at(triangles.polygons[i].vertices[0]).z
      );

      v1 = Ogre::Vector3(
                  newCloud.points.at(triangles.polygons[i].vertices[1]).x,
                  newCloud.points.at(triangles.polygons[i].vertices[1]).y,
                  newCloud.points.at(triangles.polygons[i].vertices[1]).z
      );

      v2 = Ogre::Vector3(
                  newCloud.points.at(triangles.polygons[i].vertices[2]).x,
                  newCloud.points.at(triangles.polygons[i].vertices[2]).y,
                  newCloud.points.at(triangles.polygons[i].vertices[2]).z
      );

      std::cout << "v0(" << v0.x << ", " << v0.y << ", " << v0.z << ")" << std::endl;
      std::cout << "v1(" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
      std::cout << "v2(" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;

      Ogre::Vector3 u, v;
      u = v1 - v0;
      v = v2 - v0;
      Ogre::Vector3 normal(
            (u.y * v.z) - (u.z * v.y),
            (u.z * v.x) - (u.x * v.z),
            (u.x * v.y) - (u.y * v.x)
      );

      resultStruct.normals.push_back(normal);
      resultStruct.normals.push_back(normal);
      resultStruct.normals.push_back(normal);

      resultStruct.vertices.push_back(v0);
      resultStruct.vertices.push_back(v1);
      resultStruct.vertices.push_back(v2);

      resultStruct.texCoords.push_back(Ogre::Vector2(v0.x, v0.z));
      resultStruct.texCoords.push_back(Ogre::Vector2(v1.x, v1.z));
      resultStruct.texCoords.push_back(Ogre::Vector2(v2.x, v2.z));
  }
}

} // Degenerator namespace
*/
