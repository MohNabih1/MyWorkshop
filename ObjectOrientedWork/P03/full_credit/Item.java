package store;
public class Item {
    private Product product;
    private int quantity;

    public Item(Product product, int quantity) {
        this.quantity = quantity;
        this.product = product;
    }

    public double getPrice() {
        return quantity * product.getPrice();
    }

    @Override
    public String toString() {
        return String.format("%3d %s $ %05.2f", quantity, product.toString(), getPrice());
    }
}

