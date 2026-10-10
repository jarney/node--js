
export class ConnectionData {

       mData: { [ key: string]: string };
	    
       constructor() {
           this.mData = {};
       }
       public setValue(key: string, value: string) {
           this.mData[key] = value;
       }
       public appendValue(key: string, value: string) {
           this.mData[key] = this.mData[key] + value;
       }
       public getValue(key: string, default_value: string): string {
           if (key in this.mData) {
	       return this.mData[key];
	   }
	   else {
	       return default_value;
	   }
       }
       public hasValue(key: string) : boolean {
           return key in this.mData;
       }
       public getData() : { [ key: string]: string } {
           return this.mData;
       }
       public size() : number {
           return this.mData.size();
       }
}
