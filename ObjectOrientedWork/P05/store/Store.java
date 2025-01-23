package store;
import java.util.ArrayList;

import store.Item;
import store.Customer;
import store.Tool;
import store.Plant;
import store.Exposure;

public class Store {
private String name;
private ArrayList<Customer> customers;
private ArrayList<Product> products;
private ArrayList<Order> orders;

public Store (String name) {
this.name = name;
this.customers = new ArrayList<>();
this.products = new ArrayList<>();
this.orders = new ArrayList<>();
}

public String getName () {
return this.name; 
}

// To add the customer to the ArrayList<Customer>
public void addCustomer (Customer customer) {
this.customers.add(customer);
}

// To return the list of customers formatted as a menu
public String getCustomerList (){
StringBuilder sb = new StringBuilder();
for (int i = 0; i < product.size(); i++){
sb.append(i).append(". ").append(products.get(i)).append("\n");
}
return sb.toString();
}

//To add product to the ArrayList<Product>
public void addProduct(Product product) {
this.products.add(product);
}


public String getProductList() {
StringBuilder sb = new StringBuilder();
for (int i = 0; i < products.size(); i++) {
sb.append(i).append(". ").append(products.get(i)).append("\n");
        }
return sb.toString();
}

// new Order intialization
public int newOrder ( int customerIndex) {
Order order = new Order (customer);
this.orders.add(order);
return this.orders.size() -1; 
}

// Adding itmes to the order

 public void addToOrder(int orderIndex, int productIndex, int quantity) {
        if (orderIndex >= 0 && orderIndex < orders.size() && productIndex >= 0 && productIndex < products.size()) {
            Product product = products.get(productIndex);
            Item item = new Item(product, quantity);
            orders.get(orderIndex).addItem(item);
        } 
else {
            System.out.println("Invalid entry");
 }
}

public String getOrderList() {
StringBuilder sb2 = new StringBuilder();
for ( int i = 0; i < orders.size(); i++) {
sb2.append(i).append(",").append(orders.get(i)).append("\n");
}
return sb2.toString();





}


}


