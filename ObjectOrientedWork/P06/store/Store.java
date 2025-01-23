// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package store;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.ArrayList;

import java.util.ArrayList;

public class Store {
    public Store(String name) {
        this.name = name;
        this.customers = new ArrayList<>();
        this.products  = new ArrayList<>();
        this.orders    = new ArrayList<>();
    }

// To Read from BufferReader
public Store(BufferedReader reader) throws IOException {
        this.name = reader.readLine(); 
        this.customers = new ArrayList<>();
        this.products = new ArrayList<>(); 
        this.orders = new ArrayList<>(); 

        // Load customers
        int numCustomers = Integer.parseInt(reader.readLine()); 
        for (int i = 0; i < numCustomers; ++i) {
            this.customers.add(new Customer(reader)); 
        }
 int numProducts = Integer.parseInt(reader.readLine());
        for (int i = 0; i < numProducts; ++i) {
            String productType = reader.readLine();
            Product product = null;
            if ("store.Plant".equals(productType)) {
                product = new Plant(reader);
            } else if ("store.Tool".equals(productType)) {
                product = new Tool(reader);
            }
            if (product != null) {
                this.products.add(product);
            }
        }
    }


// Saveing the Customer info to the the BufferWriter
    public void save(BufferedWriter writer) throws IOException {
        writer.write(name);
        writer.newLine(); 
        
        writer.write(String.valueOf(customers.size()));
        writer.newLine(); 
        for (Customer customer : customers) {
            customer.save(writer); 
        }

 writer.write(Integer.toString(products.size()));
        writer.newLine();
        for (Product product : products) {
            writer.write(product.getClass().getName());
            writer.newLine();
            product.save(writer);
        }
        
   
      
    }
    public String getName() {
        return name;
    }
    // Customers
    public void addCustomer(Customer customer) {
        customers.add(customer);
    }
    
    public String getCustomerList() {
        StringBuilder sb = new StringBuilder();
        for(int i=0; i<customers.size(); ++i) 
            sb.append(String.format("%3d] %s\n", i, customers.get(i)));
        return sb.toString();
    }
    
    // Products
    public void addProduct(Product product) {
        products.add(product);
    }
    
    public String getProductList() {
        StringBuilder sb = new StringBuilder();
        for(int i=0; i<products.size(); ++i) 
            sb.append(String.format("%3d] %s\n", i, products.get(i)));
        return sb.toString();
    }
    
    // Orders
    public int newOrder(int customer) {
        orders.add(new Order(customers.get(customer)));
        return orders.size() - 1;
    }
    public void addToOrder(int order, int product, int quantity) {
        orders.get(order).addItem(new Item(products.get(product), quantity));
    }
   
    public String getOrderList() {
        StringBuilder sb = new StringBuilder();
        for(int i=0; i<orders.size(); ++i) 
            sb.append(String.format("\n%s\n", orders.get(i)));
        return sb.toString();
    }
   
    private String name;
    private ArrayList<Customer> customers;
    private ArrayList<Product> products;
    private ArrayList<Order> orders;
}


