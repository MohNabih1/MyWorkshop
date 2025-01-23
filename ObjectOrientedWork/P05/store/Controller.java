package store;
import java.util.Scanner;

import store.Item;
import store.Customer;
import store.Tool;
import store.Plant;
import store.Exposure;

public class Controller {
    private Store store;
    private View view;
    private Menu mainMenu;
    private String output;
    private boolean isRunning;
    private Scanner in;

    public Controller(Store store) {
        this.store = store;
        this.view = View.MAIN_MENU;
        this.mainMenu = new Menu("Main Menu");
        this.output = "";
        this.isRunning = true;
        this.in = new Scanner(System.in);

mainMenu.addMenuItem(new MenuItem("Exit", this::exit));
mainMenu.addMenuItem(new MenuItem("Hello,New Customer", this::newCustomer));
mainMenu.addMenuItem(new MenuItem("Define New Tool", this::newTool));
mainMenu.addMenuItem(new MenuItem("Define New Plant", this::newPlant));
mainMenu.addMenuItem(new MenuItem("Place Order", this::placeOrder));
mainMenu.addMenuItem(new MenuItem("Switch View", this::switchView));
    }

public void mdi() {
   while (isRunning) {
     try {
                selectFromMenu();
                output = "";
                mainMenu.run(getInt("Select an option: "));
         } catch (Exception e) {
                System.err.println("Error! " + e.getMessage());
       }
      }
  }

 private void exit() {
        isRunning = false;
    }
private void placeOrder() {
        System.out.println(store.getCustomerList());
        int customerIndex = getInt("Select customer: ");
        Customer customer = store.getCustomer(customerIndex);
        int orderIndex = store.newOrder(customer);
        while (true) {
            System.out.println(store.getProductList());
            int productIndex = getInt("Select product (-1 to finish): ");
            if (productIndex == -1) break;
            int quantity = getInt("Enter quantity: ");
            store.addToOrder(orderIndex, productIndex, quantity);
        }
        output = "Order placed successfully.";
        view = View.ORDERS;
    }

    private void newCustomer() {
        String name = getString("Enter the customer's name: ");
        String email = getString("Enter email address: ");
        store.addCustomer(new Customer(name, email));
        output = "Customer had been added successfully.";
        view = View.CUSTOMERS;
    }

    private void newTool() {
        String name = getString("Enter tool's name: ");
        double price = getDouble("Enter tool's price: ");
        store.addProduct(new Tool(name, price));
        output = "Tool had been added successfully.";
        view = View.PRODUCTS;
    }

    private void newPlant() {
        String name = getString("Enter plant's name: ");
        double price = getDouble("Enter plant's price: ");
        Exposure exposure = chooseExposure();
        store.addProduct(new Plant(name, exposure, price));
        output = "Plant has been added successfully.";
        view = View.PRODUCTS;
    }


    private void switchView() {
        System.out.println("Available views: ");
        for (View v : View.values()) {
            System.out.println(v.ordinal() + ". " + v);
        }
        int choice = getInt("Enter view number: ");
        view = View.values()[choice];
    }

// prints the prompt
private String getString(String prompt) {
    while (true) {
        try {
            System.out.print(prompt);
            String input = in.nextLine().trim();
            if (input.isEmpty()) {
                throw new IllegalArgumentException();
            }
            return input;
        } catch (IllegalArgumentException e) {
            System.err.println("Invalid input!Try again");
  }
 }
}


   

    private int getInt(String prompt) {
   while (true) {
            try {
                String input = getString(prompt);
                return Integer.parseInt(input);
          } catch (NumberFormatException e) {
                System.err.println("Invalid input, enter an integer.");
          }
    }
  }

    private double getDouble(String prompt) {
        while (true) {
            try {
                String input = getString(prompt);
                return Double.parseDouble(input);
          } catch (NumberFormatException e) {
               System.err.println("Invalid inpu, enter a number.");
          }
      }
   }


    private void selectFromMenu() {
        clearScreen();
        System.out.println(store.getName());
        System.out.println(mainMenu);
        switch (view) {
            case CUSTOMERS:
                System.out.println(store.getCustomerList());
                break;
            case PRODUCTS:
                System.out.println(store.getProductList());
                break;
            case ORDERS:
                System.out.println(store.getOrderList());
                break;
        }
        System.out.println(output);
    }
    }

}


       
