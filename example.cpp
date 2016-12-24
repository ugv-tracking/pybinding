#include <pybind11/pybind11.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/filter.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>
#include <pcl/io/ply_io.h>


typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloud;
typedef PointCloud::Ptr PointCloudPtr;

namespace py = pybind11;
int add(int i, int j) {
    return i + j;
}

class CLOUD {
public:
    CLOUD()
    {
        cloud_ = PointCloudPtr (new PointCloud);
    }
    virtual ~CLOUD(){}

    void reset()
    {
        cloud_->clear();
        return;
    }

    void genCloud(int num)
    {
        for (size_t i = 0; i < num; i++){
            PointT point;
            point.x = i;
            point.y = i+1;
            point.z = i+2;
            cloud_->push_back(point);
        }
        std::cout << "Pointcloud size is " << cloud_->size() << std::endl;
        for (size_t i  = 0 ; i < cloud_->size(); i++)
            std::cout << cloud_->points[i] << std::endl;
        return;
    }

private:
    PointCloudPtr cloud_;
};

class Pet {
public:
    Pet(const std::string &name, const std::string &species)
        : m_name(name), m_species(species) {
        std::cout << "funny" << std::endl;
    }
    std::string name() const { return m_name; }
    std::string species() const { return m_species; }
private:
    std::string m_name;
    std::string m_species;
};

class Dog : public Pet {
public:
    Dog(const std::string &name) : Pet(name, "dog") {}
    std::string bark() const { return "Woof!"; }
};

class Rabbit : public Pet {
public:
    Rabbit(const std::string &name) : Pet(name, "parrot") {}
};

class Hamster : public Pet {
public:
    Hamster(const std::string &name) : Pet(name, "rodent") {}
};


std::string pet_name_species(const Pet &pet) {
    return pet.name() + " is a " + pet.species();
}

std::string dog_bark(const Dog &dog) {
    return dog.bark();
}


PYBIND11_PLUGIN(example) {
    py::module m("example", "pybind11 example plugin");

    m.def("add", &add, "A function which adds two numbers");
    py::class_<CLOUD>(m, "CLOUD")
            .def(py::init<>())
            .def("reset", &CLOUD::reset)
            .def("genCloud", &CLOUD::genCloud);

    py::class_<Pet> pet_class(m, "Pet");
    pet_class
        .def(py::init<std::string, std::string>())
        .def("name", &Pet::name)
        .def("species", &Pet::species);

    /* One way of declaring a subclass relationship: reference parent's class_ object */
    py::class_<Dog>(m, "Dog", pet_class)
        .def(py::init<std::string>())
        .def("bark", &Dog::bark);

    /* Another way of declaring a subclass relationship: reference parent's C++ type */
    py::class_<Rabbit>(m, "Rabbit", pet_class)
        .def(py::init<std::string>());

    /* And another: list parent in class template arguments */
    py::class_<Hamster>(m, "Hamster", pet_class)
        .def(py::init<std::string>());

    m.def("pet_name_species", pet_name_species);
    m.def("dog_bark", dog_bark);

    return m.ptr();
}
