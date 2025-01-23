package store;

public abstract class Product {
    private static int nextStockNumber = 0;
    private int stockNumber;
    private double price;
    private String name;

    public Product(String name, double price) {
        if (price < 0) {
            throw new IllegalArgumentException("Invalid price of " + name + ": " + price);
        }

        this.name = name;
        this.price = price;
        this.stockNumber = nextStockNumber++;
    }

    public int getStockNumber() {
        return stockNumber;
    }

    public double getPrice() {
        return price;
    }

    @Override
    public String toString() {
        return String.format("%-30s $ %05.2f", name, price);
    }
}

