#include "store.hpp"

FoodStore::FoodStore(std::string name): //, std::vector<Food> foods):
    m_name {std::move(name)}
    // m_food {std::move(foods)}
{
}

unsigned FoodStore::getNumberOfNutritions() const
{
    if (m_food.size() == 0)
        return 0;
    else
        return m_food[0]->getNumberOfNutritions();
}

// std::vector<std::reference_wrapper<Food>> FoodStore::getAllGoodReferences() const
// {
//     std::vector<std::reference_wrapper<Food>> foodRefs;
//     for (const auto& ptr : m_food)
//         foodRefs.push_back(std::ref(*ptr));
//     return foodRefs;
// }

std::vector<double> FoodStore::getFoodPropertyVector(std::string property) const
{
    std::vector<double> propertyVector;
    if (property == "min")
        for (const auto& ptr : m_food)
            propertyVector.push_back(ptr->getMin());
    else if (property == "max")
        for (const auto& ptr : m_food)
            propertyVector.push_back(ptr->getMax());
    else if (property == "cost")
        for (const auto& ptr : m_food)
            propertyVector.push_back(ptr->getCost());
    else if (property == "amount")
        for (const auto& ptr : m_food)
            propertyVector.push_back(ptr->getAmount());
    return propertyVector;
}

std::vector<std::vector<double>> FoodStore::getFoodNutritionValues() const
{
    std::vector<std::vector<double>> propertyVector;
    for (const auto& ptr : m_food)
        propertyVector.push_back(ptr->getNutritionValues());
    return propertyVector;
}

void FoodStore::decorateWithCost(const std::vector<double>& newVariableVector)
{
    if (newVariableVector.size() == getNumberOfFoods())
        for (unsigned i=0; i<newVariableVector.size(); i++)
            m_food[i] = std::make_unique<Cost>(std::move(m_food[i]), newVariableVector[i]);
}

void FoodStore::decorateWithAmount(const std::vector<double>& newVariableVector)
{
    if (newVariableVector.size() == getNumberOfFoods())
        for (unsigned i=0; i<newVariableVector.size(); i++)
            m_food[i] = std::make_unique<Amount>(std::move(m_food[i]), newVariableVector[i]);
}

void FoodStore::readFromJson(const Json::Value& storeObj)
{
    m_name = storeObj["name"].asString();

    clearFood();
    const Json::Value& foods = storeObj["foods"];
    for (const Json::Value& foodObj : foods)
    {
        Food food;
        food.readFromJson(foodObj);
        addGood(std::move(food));
    }
}

// void FoodStore::readFromJson(std::unique_prt<Food> foodPtr, const Json::Value& storeObj)
// {
//     m_name = storeObj["name"].asString();

//     clearFood();
//     const Json::Value& foods = storeObj["foods"];
//     for (const Json::Value& foodObj : foods)
//     {
//         foodPtr.readFromJson(foodObj);
//         addGood(std::move(foodPtr));
//     }
// }

Json::Value FoodStore::toJson() const
{
    Json::Value foods;
    for (const auto& food : m_food)
        foods.append(food->toJson());
    
    Json::Value storeObj;
    storeObj["name"] = m_name;
    storeObj["foods"] = foods;

    return storeObj;
}
