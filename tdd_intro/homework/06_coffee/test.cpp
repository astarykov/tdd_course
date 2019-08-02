/*
We have to develop a coffee machine with TDD. The machine uses the automated source of ingredients, which implements the interface ISourceOfIngredients.

Our machine should be able to produce the coffee accordingly to the next receipts:
- americano: water & coffee 1:2 or 1:3. Water temp 60C
- cappuccino - milk & coffee & milk foam 1:3, 1:3, 1:3. Water temp 80C
- latte - milk & coffee & milk foam 1:4, 1:2, 1:4. Water temp 90C
- marochino - chocolate & coffee & milk foam, 1:4, 1:4, 1:4 and 1:4 is empty

We have 2 possible sizes of the cup:
- little 100 gram
- big 140 gram

Implement worked coffee machine using ISourceOfIngredients to controll the process of coffee production.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ISourceOfIngredients
{
public:
    virtual ~ISourceOfIngredients() {}
    virtual void SetCupSize(int gram) = 0;
    virtual void AddWater(int gram, int temperature) = 0;
    virtual void AddSugar(int gram) = 0;
    virtual void AddCoffee(int gram) = 0;
    virtual void AddMilk(int gram) = 0;
    virtual void AddMilkFoam(int gram) = 0;
    virtual void AddChocolate(int gram) = 0;
    virtual void AddCream(int gram) = 0;


};

class SourceOfIngredientsMock: public ISourceOfIngredients {
    public:
        MOCK_METHOD1(SetCupSize, void(int));
        MOCK_METHOD2(AddWater, void(int, int));
        MOCK_METHOD1(AddSugar, void(int));
        MOCK_METHOD1(AddCoffee, void(int));
        MOCK_METHOD1(AddMilk, void(int));
        MOCK_METHOD1(AddMilkFoam, void(int));
        MOCK_METHOD1(AddChocolate, void(int));
        MOCK_METHOD1(AddCream, void(int));
};

class CofeeMachine {
    public:
    CofeeMachine(ISourceOfIngredients *ingredientSource) {
        _ingredientSource = ingredientSource;
    }

    void makeAmericano(int capacity) {
        _ingredientSource->SetCupSize(capacity);
        _ingredientSource->AddWater(capacity/2, 60);
        _ingredientSource->AddCoffee(capacity/2);
    }

    void makeCapuchino(int capacity) {
        _ingredientSource->SetCupSize(capacity);
        _ingredientSource->AddWater(0, 80);
        _ingredientSource->AddCoffee(capacity/3);
        _ingredientSource->AddMilk(capacity/3);
        _ingredientSource->AddMilkFoam(capacity/3);
    }

    void makeLatte(int capacity) {
        _ingredientSource->SetCupSize(capacity);
    }

    void makeMarochino(int capacity) {
        _ingredientSource->SetCupSize(capacity);
    }



    private:
        ISourceOfIngredients *_ingredientSource;
};


TEST(Coffee, makeAmericanoOfSizeSmall) {
    SourceOfIngredientsMock source;
    CofeeMachine machine(&source);

    EXPECT_CALL(source, SetCupSize(100)).Times(1);
    EXPECT_CALL(source, AddWater(50, 60)).Times(1);
    EXPECT_CALL(source, AddCoffee(50)).Times(1);
    machine.makeAmericano(100);
}

TEST(Coffee, makeAmericanoOfSizeBig) {
    SourceOfIngredientsMock source;
    CofeeMachine machine(&source);

    EXPECT_CALL(source, SetCupSize(140)).Times(1);
    EXPECT_CALL(source, AddWater(70, 60)).Times(1);
    EXPECT_CALL(source, AddCoffee(70)).Times(1);
    machine.makeAmericano(140);
}

TEST(Coffee, makeCapuchinoOfSizeSmall) {
    SourceOfIngredientsMock source;
    CofeeMachine machine(&source);

    EXPECT_CALL(source, SetCupSize(100)).Times(1);
    EXPECT_CALL(source, AddMilkFoam(100/3)).Times(1);
    EXPECT_CALL(source, AddWater(0, 80)).Times(1);
    EXPECT_CALL(source, AddCoffee(100/3)).Times(1);
    EXPECT_CALL(source, AddMilk(100/3)).Times(1);
    machine.makeCapuchino(100);
}

TEST(Coffee, makeCapuchinoOfSizeBig) {
    SourceOfIngredientsMock source;
    CofeeMachine machine(&source);

    EXPECT_CALL(source, SetCupSize(140)).Times(1);
    EXPECT_CALL(source, AddMilkFoam(140/3)).Times(1);
    EXPECT_CALL(source, AddWater(0, 80)).Times(1);
    EXPECT_CALL(source, AddCoffee(140/3)).Times(1);
    EXPECT_CALL(source, AddMilk(140/3)).Times(1);
    machine.makeCapuchino(140);
}

TEST(Coffee, makeLatteOfSizeSmall) {
    SourceOfIngredientsMock source;
    CofeeMachine machine(&source);

    EXPECT_CALL(source, SetCupSize(100)).Times(1);
    EXPECT_CALL(source, AddWater(0, 90)).Times(1);
    EXPECT_CALL(source, AddCoffee(100/2)).Times(1);
    EXPECT_CALL(source, AddMilk(100/4)).Times(1);
    EXPECT_CALL(source, AddMilkFoam(100/4)).Times(1);
    machine.makeLatte(100);
}
