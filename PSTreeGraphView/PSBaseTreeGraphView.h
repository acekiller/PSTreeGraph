//
//  PSBaseTreeGraphView.h
//  PSTreeGraphView
//
//  Created by Ed Preston on 7/25/10.
//  Copyright 2010 Preston Software. All rights reserved.
//
//
// This is a port of the sample code from Max OS X to iOS (iPad).
//
// WWDC 2010 Session 141, “Crafting Custom Cocoa Views”
//


#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>



// A TreeGraph's nodes may be connected by either "direct" or "orthogonal" lines.
 
enum PSTreeGraphConnectingLineStyle {
    PSTreeGraphConnectingLineStyleDirect = 0,
    PSTreeGraphConnectingLineStyleOrthogonal = 1
};
typedef NSInteger PSTreeGraphConnectingLineStyle;


// A TreeGraph's orientation may be either "horizontal" or "vertical".

enum PSTreeGraphOrientationStyle {
    PSTreeGraphOrientationStyleHorizontal = 0,
    PSTreeGraphOrientationStyleVertical = 1
};
typedef NSInteger PSTreeGraphOrientationStyle;



@class PSBaseSubtreeView;


@protocol PSTreeGraphModelNode;
@protocol PSTreeGraphDelegate;



@interface PSBaseTreeGraphView : UIView {
    
@private
	// Model
    id <PSTreeGraphModelNode> modelRoot;
	
	// Delegate
	id <PSTreeGraphDelegate> delegate;
	
    // Model Object -> SubtreeView Mapping
	NSMutableDictionary *modelNodeToSubtreeViewMapTable;
	
    // Node View Nib Specification
    NSString *nodeViewNibName;
    NSBundle *nodeViewNibBundle;
	
    // Selection State
    NSMutableSet *selectedModelNodes;
	
    // Layout State
    CGSize minimumFrameSize;
	
    // Animation Support
    BOOL animatesLayout;
    BOOL layoutAnimationSuppressed;
	
    // Layout Metrics
    CGFloat contentMargin;
    CGFloat parentChildSpacing;
    CGFloat siblingSpacing;
	
    // Layout Behavior
    BOOL resizesToFillEnclosingScrollView;
	PSTreeGraphOrientationStyle treeGraphOrientation;
	
    // Styling
    // UIColor *backgroundColor;
	
    UIColor *connectingLineColor;
    CGFloat connectingLineWidth;
    PSTreeGraphConnectingLineStyle connectingLineStyle;
	
    BOOL showsSubtreeFrames;
	
	// iOS 4 and above ONLY
    // UINib *cachedNodeViewNib;
	
}


#pragma mark -
#pragma mark Delegate

@property (nonatomic, assign) id delegate;


#pragma mark -
#pragma mark Parent Resize Notification

// Use this method to keep the view in sync for now.

- (void)parentClipViewDidResize:(id)object;


#pragma mark -
#pragma mark Creating Instances

// Initializes a new TreeGraph instance.  (TreeGraph's designated initializer is the same as 
// UIView's: -initWithFrame:.)  The TreeGraph has default appearance properties and layout 
// metrics, but to have a usable TreeGraph with actual content, you need to specify a 
// nodeViewNibName, an optional nodeViewNibBundle, and a modelRoot.
 
- (id)initWithFrame:(CGRect)frame;


#pragma mark -
#pragma mark Connection to Model

// The root of the model node tree that the TreeGraph is being asked to display.  (The modelRoot
// may have ancestor nodes, but TreeGraph will ignore them and treat modelRoot as the root.)  May
// be set to nil, in which case the TreeGraph displays no content.  The modelRoot object, and all
// of its desdendants as exposed through recursive application of the "-childModelNodes" accessor
// to traverse the model tree, must conform to the TreeGraphModelNode protocol declared in 
// TreeGraphModelNode.h
 
@property(retain) id <PSTreeGraphModelNode> modelRoot;


#pragma mark -
#pragma mark Root SubtreeView Access

// A TreeGraph builds the tree it displays using recursively nested SubtreeView instances.  This
// read-only accessor provides a way to get the rootmost SubtreeView (the one that corresponds 
// to the modelRoot model node).
 
@property(readonly) PSBaseSubtreeView *rootSubtreeView;


#pragma mark -
#pragma mark Node View Nib Specification

// The name of the .nib file from which to instantiate node views.  (This API design assumes that
// all node views should be instantiated from the same .nib.  If a tree of heterogeneous nodes 
// was desired, we could switch to a different mechanism for identifying the .nib to instantiate.)
// Must specify a "View" .nib file, whose File's Owner is a SubtreeView, or the TreeGraph will be
// unable to instantiate node views.
 
@property(copy) NSString *nodeViewNibName;

// The NSBundle from which the .nib named by nodeViewNibName can be loaded.  May be nil, in which
// case we follow the usual UIKit-implemented rules for automatically finding the named .nib 
// file.
 
@property(retain) NSBundle *nodeViewNibBundle;


#pragma mark -
#pragma mark Selection State

//  The unordered set of model nodes that are currently selected in the TreeGraph.  When no nodes
// are selected, this is an empty NSSet.  It will never be nil (and attempting to set it to nil
// will raise an exception).  Every member of this set must be a descendant of the TreeGraph's 
// modelRoot (or modelRoot itself).  If any member is not, TreeGraph will raise an exception.
 
@property(copy) NSSet *selectedModelNodes;

// Convenience accessor that returns the selected node, if exactly one node is currently 
// selected.  Returns nil if zero, or more than one, nodees are currently selected.
 
@property(readonly) id <PSTreeGraphModelNode> singleSelectedModelNode;

// Returns the bounding box of the selectedModelNodes.  The bounding box takes only the selected
// nodes into account, disregarding any descendants they might have.
 
@property(readonly) CGRect selectionBounds;


#pragma mark -
#pragma mark Node Hit-Testing

// Returns the model node under the given point, which must be expressed in the TreeGraph's 
// interior (bounds) coordinate space.  If there is a collapsed subtree at the given point, 
// returns the model node at the root of the collapsed subtree.  If there is no model node 
// at the given point, returns nil.
 
- (id <PSTreeGraphModelNode> )modelNodeAtPoint:(CGPoint)p;


#pragma mark -
#pragma mark Sizing and Layout

// A TreeGraph's minimumFrameSize is the size needed to accommodate its content (as currently 
// laid out) and margins.  Changes to the TreeGraph's content, layout, or margins will update 
// this.  When a TreeGraph is the documentView of an NSScrollView, its actual frame may be larger
// than its minimumFrameSize, since we automatically expand the TreeGraph to always be at least 
// as large as the NSScrollView's clip area (contentView) to provide a nicer user experience.
 
@property CGSize minimumFrameSize;

// If YES, and if the TreeGraph is the documentView of an UIScrollView, the TreeGraph will 
// automatically resize itself as needed to ensure that it always at least fills the content 
// area of its enclosing UIScrollView.  If NO, or if the TreeGraph is not the documentView of 
// an UIScrollView, the TreeGraph's size is determined only by its content and margins.
 
@property BOOL resizesToFillEnclosingScrollView;

// The style for tree graph orientation  (See the TreeGraphOrientationStyle enumeration above.)

@property PSTreeGraphOrientationStyle treeGraphOrientation;

//Returns YES if the tree needs relayout.
 
- (BOOL)needsGraphLayout;

//Marks the tree as needing relayout.
 
- (void)setNeedsGraphLayout;

// Performs graph layout, if the tree is marked as needing it.  Returns the size computed for the
// tree (not including contentMargin).
 
- (CGSize)layoutGraphIfNeeded;

// Collapses the root node, if it is currently expanded.
 
- (void)collapseRoot;

// Expands the root node, if it is currently collapsed.
 
- (void)expandRoot;

// Toggles the expansion state of the TreeGraph's selectedModelNodes, expanding those that are
// currently collapsed, and collapsing those that are currently expanded.
 
- (IBAction)toggleExpansionOfSelectedModelNodes:(id)sender;

// Returns the bounding box of the node views that represent the specified modelNodes.  Model 
// nodes that aren't part of the displayed tree, or are part of a collapsed subtree, are ignored
// and don't contribute to the returned bounding box.  The bounding box takes only the specified
// nodes into account, disregarding any descendants they might have.
 
- (CGRect)boundsOfModelNodes:(NSSet *)modelNodes;


#pragma mark -
#pragma mark Scrolling

// Does a [self scrollRectToVisible:] with the bounding box of the specified model nodes.
 
- (void)scrollModelNodesToVisible:(NSSet *)modelNodes;

// Does a [self scrollRectToVisible:] with the bounding box of the selected model nodes.
 
- (void)scrollSelectedModelNodesToVisible;


#pragma mark -
#pragma mark Animation Support

// Whether the TreeGraph animates layout operations.  Defaults to YES.  If set to NO, layout 
// jumpst instantaneously to the tree's new state.
 
@property BOOL animatesLayout;

// Used to temporarily suppress layout animation during event tracking.  Layout animation happens
// only if animatesLayout is YES and this is NO.
 
@property BOOL layoutAnimationSuppressed;


#pragma mark -
#pragma mark Layout Metrics

// The amount of padding to leave between the displayed tree and each of the four edges of the 
// TreeGraph's bounds.
 
@property CGFloat contentMargin;

// The horizonal spacing between each parent node and its child nodes.
 
@property CGFloat parentChildSpacing;

// The vertical spacing betwen sibling nodes.
 
@property CGFloat siblingSpacing;


#pragma mark -
#pragma mark Styling

// The fill color for the TreeGraph's content area.
 
// @property(copy) UIColor *backgroundColor;

// The stroke color for node connecting lines.
 
@property(copy) UIColor *connectingLineColor;

// The width for node connecting lines.
 
@property CGFloat connectingLineWidth;

// The style for node connecting lines.  (See the PSTreeGraphConnectingLineStyle enumeration above.)
 
@property PSTreeGraphConnectingLineStyle connectingLineStyle;

// Defaults to NO.  If YES, a stroked outline is shown around each of the TreeGraph's 
// SubtreeViews.  This can be helpful for visualizing the TreeGraph's structure and layout.
 
@property BOOL showsSubtreeFrames;


@end