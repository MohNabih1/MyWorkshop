package store;

import java.util.ArrayList;

/**
  * Encapsulates a Customer and the items they want to but in an order of items
  * @author    Mohamad Nabih Alkhateeb
  * @version   0.2
  * @since     0.2

*/

public class Order {

// Customer values can't be changed 
    private static int nextOrderNumber = 1;
    private final int orderNumber;
    private final Customer customer;
    private final ArrayList<Item> items;


/**
  *@parm customer   the customer placing the order 
  *@since  0.2

*/


    public Order(Customer customer) {
        this.orderNumber = nextOrderNumber++;
        this.customer = customer;
        this.items = new ArrayList<>();
    }


/**
  * to add an item to the order 
  *@parm item    the item to be added to the order
  *@since 0.2 

*/
  

// To add an item to the order 
    public void addItem(Item item) {
        items.add(item);
    }

/**
  * sums up the prices of the items 
  *returns total   the grand total of the items of the order
  *since 0.2

*/



// Return the sume of prices
    public double getPrice() {
        double total = 0;
        for (Item item : items) {
            total += item.getPrice();
        }
        return total;
    }

/**
  * prints a recipt format output 
  *@returns receipt.toString   A formatted string representaion of the order 
  *@since 0.2

*/

 
// To print a recipt format output 
    @Override
    public String toString() {
        StringBuilder receipt = new StringBuilder();
        receipt.append("Order Number: ").append(orderNumber).append("\n");
        receipt.append("Customer: ").append(customer.getName()).append("\n");
        receipt.append("Items:\n");
       for (Item item : items) {
    receipt.append("- ").append(item).append(": $").append(item.getPrice()).append("\n");
}

        receipt.append("\nTotal Price: $").append(getPrice());
        return receipt.toString();
    }
}

