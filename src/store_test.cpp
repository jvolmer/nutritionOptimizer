#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE storetest

#include "food.hpp"
#include "store.hpp"
#include <json/json.h>
#include <boost/test/unit_test.hpp>
#include <utility>
#include <fstream>
#include <vector>

BOOST_AUTO_TEST_SUITE (initialization)

BOOST_AUTO_TEST_CASE( addOneFood )
{
    Food testFoodIn{"test"};

    FoodStore store{"kueche"};
    store.addGood(testFoodIn);

    BOOST_TEST( store.getNumberOfFoods() == 1);
    BOOST_TEST( store.containsFoodAtPosition(0, testFoodIn) );
}

BOOST_AUTO_TEST_CASE( addTwoFoods )
{
    Food testFoodIn1{"test1"};
    Food testFoodIn2{"test2"};
    
    FoodStore store{"kueche"};
    store.addGood(testFoodIn1);
    store.addGood(testFoodIn2);

    BOOST_TEST( store.getNumberOfFoods() == 2 );
    BOOST_TEST( store.containsFoodAtPosition(0, testFoodIn1) );
    BOOST_TEST( store.containsFoodAtPosition(1, testFoodIn2) );
}

BOOST_AUTO_TEST_SUITE_END( )


BOOST_AUTO_TEST_SUITE ( foodPropertyGetter )

BOOST_AUTO_TEST_CASE( getVectorPropertyMin )
{
    FoodStore store{"leckerSchmecker"};
    Food food1{"moehre", {4.1, 2}, 1.1, 9};
    Food food2{"brot", {5, 3}, 1, 9};

    const std::vector<double> actual = {food1.getMin(), food2.getMin()};

    store.addGood(std::move(food1));
    store.addGood(std::move(food2));
    
    const std::vector<double> expected{store.getFoodPropertyVector("min")};

    BOOST_TEST( expected == actual );
}

BOOST_AUTO_TEST_CASE( getVectorVectorPropertyNutrition )
{
    FoodStore store{"leckerSchmecker"};
    Food food1{"moehre", {4.1, 2}, 1.1, 9};
    Food food2{"brot", {5, 3}, 1, 9};

    const std::vector<std::vector<double>> actual{{4.1, 2}, {5, 3}};

    store.addGood(std::move(food1));
    store.addGood(std::move(food2));
    
    const std::vector<std::vector<double>> expected{store.getFoodNutritionValues()};

    BOOST_TEST( expected == actual);
}

BOOST_AUTO_TEST_SUITE_END( )


BOOST_AUTO_TEST_SUITE ( decorator )

BOOST_AUTO_TEST_CASE( amountDecorator )
{
    FoodStore store{"leckerSchmecker"};
    Food food{"moehre", {4.1, 2}, 1.1, 9};

    const std::vector<double> actual{10};

    store.addGood(std::move(food));
    store.decorateWithAmount(actual);

    const std::vector<double> expected{store.getFoodPropertyVector("amount")};
    
    BOOST_TEST( expected == actual );
}

BOOST_AUTO_TEST_CASE( costDecoratorForTwoFoods )
{
    FoodStore store{"leckerSchmecker"};
    Food food{"moehre", {4.1, 2}, 1.1, 9};
    Food food2{"banane", {3, 10}, 1.1, 9};

    const std::vector<double> actual{10, 11};

    store.addGood(std::move(food));
    store.addGood(std::move(food2));
    store.decorateWithCost(actual);

    const std::vector<double> expected{store.getFoodPropertyVector("cost")};
    
    BOOST_TEST( expected == actual );
}

BOOST_AUTO_TEST_CASE( costDecoratorGetNutritions )
{
    FoodStore store{"leckerSchmecker"};
    Food food{"moehre", {4.1, 2}, 1.1, 9};

    const std::vector<std::vector<double>> actual{{4.1, 2}};
    
    store.addGood(std::move(food));
    store.decorateWithCost({11});

    const std::vector<std::vector<double>> expected{store.getFoodNutritionValues()};
    
    BOOST_TEST( expected == actual );
}

BOOST_AUTO_TEST_SUITE_END( )


BOOST_AUTO_TEST_SUITE ( writeAndReadJson )

BOOST_AUTO_TEST_CASE( testReadAndWrite )
{
    // create store
    FoodStore store{"testStore"};
    Food foodIn{"testFood", {3.221, 4.3, 9.99992}, 4.3, 999.91};
    store.addGood(foodIn);

    // write store to file
    std::ofstream fileIn("test.out");
    fileIn << store.toJson();
    fileIn.close();

    // clear store
    store.clearFood();

    //read store from file
    std::ifstream fileOut("test.out");    
    Json::Value fileContent;
    fileOut >> fileContent;
    store.readFromJson(fileContent);

    // test
    BOOST_TEST( store.getNumberOfFoods() == 1 );
    BOOST_TEST( store.containsFoodAtPosition(0, foodIn) );
}

BOOST_AUTO_TEST_CASE( testWriteAmountDecoratorToJson )
{
    FoodStore store{"testStore"};
    Food foodIn{"testFood", {3.221, 4.3, 9.99992}, 4.3, 999.91};
    store.addGood(foodIn);
    store.decorateWithAmount({.3});

    Json::Value output = store.toJson();

    // check if first food in store includes amount variabel
    BOOST_TEST( output["foods"][0].isMember("amount") );
}

BOOST_AUTO_TEST_SUITE_END( )

