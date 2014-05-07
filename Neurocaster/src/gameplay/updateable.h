#ifndef UPDATEABLE_H
#define UPDATEABLE_H

class Updateable
{
public:
    Updateable();

    /**
     * @brief update_turn This is received from the main update loop.
     * @param turn_number This is the OVERALL turn number.
     * It is converted internally by the ability class, which keeps track of which turn the ability was created on.
     */
    virtual void update_turn(double turn_number) = 0;

    /**
     * @brief continuous_update Continuous update loop, for things like animations
     */
    virtual void continuous_update() = 0;

    /**
     * @brief update_action Called after each turn of completion.
     */
    virtual void update_action() = 0;

    /**
     * @brief complete_action Called when the ability completes.
     */
    virtual void complete_action() = 0;
};

#endif // UPDATEABLE_H
