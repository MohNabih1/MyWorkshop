package test;


import store.Item;
import store.Customer;
import store.Tool;
import store.Plant;
import store.Exposure;


public class TestOutput {
    public static void main(String[] args) {
        try {
            Customer c1 = new Customer("Prof Rice", "george.rice@uta.edu");
            Customer c2 = new Customer("President Joe Biden", "president@whitehouse.gov");
            Customer c3 = new Customer("The Late Queen Elizabeth II", "queen@royal.gov.uk");
            Customer c4 = new Customer("Mark Zuckerberg", "mark.zuckerberg@facebook.com");

            System.out.println("Famous customers include\n  "
                             + c1 + "\n  " + c2 + "\n  "+ c3 + "\n  " + c4);

            Tool p1 = new Tool("Cactus Cereus Peruvianus", 4990);
           Tool p2 = new Tool("'White Princess' Philodendron", 5500);
           Tool p3 = new Tool("6 inch English Lavender", 2909);
            Tool p4 = new Tool("'Hayi' Rhaphidophora", 3299);

            System.out.println("Our best plants include:\n  "
                             + p1 + "\n  " + p2 + "\n  "+ p3 + "\n  " + p4);

            Item i1 = new Item(p1, 1);
            Item i2 = new Item(p2, 2);
            Item i3 = new Item(p3, 3);
            Item i4 = new Item(p4, 4);

            System.out.println("And my order will include:\n  "
                             + i1 + "\n  " + i2 + "\n  "+ i3 + "\n  " + i4);
        } catch(Exception e) {
            System.err.println("Failed to create and print objects: \n" + e);
        }
    }
}
